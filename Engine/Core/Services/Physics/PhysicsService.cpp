#include "PhysicsService.h"
#include "Core/ECS/RigidBody2D.h"

Engine::PhysicsService::PhysicsService()
{
    m_PhysicsThread = std::jthread(&PhysicsService::PhysicsLoop, this);
}

Engine::PhysicsService::~PhysicsService()
{
    Stop();
}

void Engine::PhysicsService::AddRigidBody(RigidBody2D* rb)
{
    {
        std::unique_lock<std::mutex> lock(m_PhysicsMutex);

        m_RigidBodies.push_back(rb);

        if (!m_Collisions.contains(rb->GetEntityID()))
        {
            m_Collisions[rb->GetEntityID()] = std::set<uint64_t>();
        }
    }

    L_TRACE("Added rigid body with id: {}", rb->GetEntityID());

    m_PhysicsCondition.notify_one();
}

void Engine::PhysicsService::RemoveRigidBody(RigidBody2D* rb)
{
    {
        std::unique_lock<std::mutex> lock(m_PhysicsMutex);

	    m_RigidBodies.erase(std::remove(m_RigidBodies.begin(), m_RigidBodies.end(), rb), m_RigidBodies.end());

        // Delete from m_Collisions
        m_Collisions.erase(rb->GetEntityID());

        // Delete from other m_Collisions
        for (auto& [id, collisions] : m_Collisions)
        {
            collisions.erase(rb->GetEntityID());
        }
    }

    m_PhysicsCondition.notify_one();
}

void Engine::PhysicsService::PhysicsLoop()
{
    while (true)
    {
        {
            std::unique_lock<std::mutex> lock(m_PhysicsMutex);
            m_PhysicsCondition.wait(lock, [this]() { return m_stopRequested || !m_RigidBodies.empty(); });

            if (m_stopRequested)
                break;
        }

        Update();
    }
}

void Engine::PhysicsService::Stop()
{
    m_stopRequested = true;
    m_PhysicsCondition.notify_one();
    m_PhysicsThread.join();
}

void Engine::PhysicsService::Update()
{
    for (size_t mainRigidBodyID{}; mainRigidBodyID < m_RigidBodies.size(); ++mainRigidBodyID)
    {
        for (size_t otherRigidBodyID{}; otherRigidBodyID < m_RigidBodies.size(); ++otherRigidBodyID)
        {
            // FIX THIS LATER BY USING A POOL OF RIGID BODIES AND SETTING THE ISENABLED FLAG
            if (mainRigidBodyID >= m_RigidBodies.size() || otherRigidBodyID >= m_RigidBodies.size())
                continue;

            auto rb = m_RigidBodies[mainRigidBodyID];
            auto other = m_RigidBodies[otherRigidBodyID];

             // self ignore
            if (rb == other)
                continue;

            if (rb == nullptr || other == nullptr)
                continue;

            if (rb->GetOwner() == nullptr || other->GetOwner() == nullptr)
                continue;

            if (rb->GetOwner() == other->GetOwner())
                continue;

            if (rb->GetOwner()->IsMarkedForDeletion() || other->GetOwner()->IsMarkedForDeletion())
                continue;

            auto selfCollider = rb->GetCollider();
            auto otherCollider = other->GetCollider();

            auto selfPosition = rb->GetPosition();
            auto otherPosition = other->GetPosition();

            bool isColliding =
            (
                selfPosition.x < otherPosition.x + otherCollider.width &&
                selfPosition.x + selfCollider.width > otherPosition.x &&
                selfPosition.y < otherPosition.y + otherCollider.height &&
                selfPosition.y + selfCollider.height > otherPosition.y
            );

            // If is colliding check if it needs update or enter callback
            if (isColliding)
            {
                // If collision is already registered ands is still colliding
                if (m_Collisions.contains(rb->GetEntityID()) && m_Collisions[rb->GetEntityID()].contains(other->GetEntityID()))
                {
                    rb->OnCollision(other);
                    continue;
                }

                if (m_Collisions.contains(rb->GetEntityID()) && !m_Collisions[rb->GetEntityID()].contains(other->GetEntityID()))
                {
                    rb->OnCollisionEnter(other);
                    m_Collisions[rb->GetEntityID()].insert(other->GetEntityID());
                }
            }
            // If is not colliding check if it needs exit callback
            else
            {
                if (m_Collisions.contains(rb->GetEntityID()) && m_Collisions[rb->GetEntityID()].contains(other->GetEntityID()))
                {
                    rb->OnCollisionExit(other);
                    m_Collisions[rb->GetEntityID()].erase(other->GetEntityID());
                }
            }
        }
    }
}