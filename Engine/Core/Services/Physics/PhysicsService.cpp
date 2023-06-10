#include "PhysicsService.h"
#include "Core/ECS/RigidBody2D.h"
#include "Core/Log.h"
#include <future>

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

        m_NewBodies.push_back(rb);
        //m_RigidBodies.push_back(rb);
    }

    L_TRACE("Added rigid body with id: {}", fmt::ptr(rb));

    m_PhysicsCondition.notify_one();
}

void Engine::PhysicsService::PhysicsLoop()
{
    while (true)
    {
        {
            std::unique_lock<std::mutex> lock(m_PhysicsMutex);
            m_PhysicsCondition.wait(lock, [this]() { return m_stopRequested || !m_RigidBodies.empty() || !m_NewBodies.empty(); });

            // Add new bodies
            for (auto& rb : m_NewBodies)
            {
                m_RigidBodies.push_back(rb);
                if (!m_Collisions.contains(rb))
                {
                    m_Collisions[rb] = std::set<RigidBody2D*>();
                }
            }
            m_NewBodies.clear();

            // Remove bodies
            for (auto& rb : m_BodiesToErase)
            {
                L_DEBUG("cleaning up")
                m_RigidBodies.erase(std::remove(m_RigidBodies.begin(), m_RigidBodies.end(), rb), m_RigidBodies.end());

                // Delete from m_Collisions
                m_Collisions.erase(rb);

                // Delete from other m_Collisions
                for (auto& [id, collisions] : m_Collisions)
                {
                    collisions.erase(rb);
                }

                // Make gameobject as can be destroyed (needs to be done on main thread)
                rb->GetOwner()->SetCanBeDestroyed(true);
            }
            m_BodiesToErase.clear();

            // Stop if requested
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
    m_BodiesToErase.clear();

    for (size_t mainRigidBodyID{}; mainRigidBodyID < m_RigidBodies.size(); ++mainRigidBodyID)
    {
        RigidBody2D* rb = m_RigidBodies[mainRigidBodyID];

        if (rb->GetOwner()->IsMarkedForDeletion() && !m_BodiesToErase.contains(rb))
        {
            m_BodiesToErase.insert(rb);
            continue;
        }

        for (size_t otherRigidBodyID{}; otherRigidBodyID < m_RigidBodies.size(); ++otherRigidBodyID)
        {
            RigidBody2D* other = m_RigidBodies[otherRigidBodyID];

            if (other->GetOwner()->IsMarkedForDeletion() && !m_BodiesToErase.contains(other))
            {
                m_BodiesToErase.insert(other);
                continue;
            }

            // FIX THIS LATER BY USING A POOL OF RIGID BODIES AND SETTING THE ISENABLED FLAG
            if (mainRigidBodyID >= m_RigidBodies.size() || otherRigidBodyID >= m_RigidBodies.size())
                continue;

            

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
                // Collider will not be center based. so we offset it here
                selfPosition.x - (selfCollider.width / 2) + (selfCollider.width / 2) < otherPosition.x + (otherCollider.width / 2) + (otherCollider.width / 2) &&
                selfPosition.x + (selfCollider.width / 2) + (selfCollider.width / 2) > otherPosition.x - (otherCollider.width / 2) + (otherCollider.width / 2) &&
                selfPosition.y - (selfCollider.height / 2) + (selfCollider.width / 2) < otherPosition.y + (otherCollider.height / 2) + (otherCollider.width / 2) &&
                selfPosition.y + (selfCollider.height / 2) + (selfCollider.width / 2) > otherPosition.y - (otherCollider.height / 2) + (otherCollider.width / 2)
            );

            const bool collisionExists = (m_Collisions.contains(rb));
            const bool collisionExistsForOther = m_Collisions[rb].contains(other);

            // If collision is already registered ands is still colliding
            if (isColliding && collisionExists && collisionExistsForOther)
            {
                rb->OnCollision(other);
                continue;
            }
            // If is collidingn and is not registered
            if (isColliding && collisionExists && !collisionExistsForOther)
            {
                rb->OnCollisionEnter(other);
                m_Collisions[rb].insert(other);
                continue;
            }
            // If is not colliding check if it needs exit callback
            if (!isColliding && collisionExists && collisionExistsForOther)
            {
                rb->OnCollisionExit(other);
                m_Collisions[rb].erase(other);
                continue;
            }
        }
    }
}

void Engine::PhysicsService::CleanAll()
{
    L_DEBUG("Flushing physics service");

    std::unique_lock<std::mutex> lock(m_PhysicsMutex);

    for (auto& rb : m_RigidBodies)
    {
        rb->GetOwner()->MarkForDeletion();
    }

    m_Collisions.clear();
    m_NewBodies.clear();
}