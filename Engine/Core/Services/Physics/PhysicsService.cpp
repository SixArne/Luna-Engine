#include "PhysicsService.h"
#include "Core/ECS/RigidBody2D.h"

void Engine::PhysicsService::AddRigidBody(RigidBody2D* rb)
{
    m_RigidBodies.push_back(rb);

    if (!m_Collisions.contains(rb->GetEntityID()))
    {
        m_Collisions[rb->GetEntityID()] = std::set<uint64_t>();
    }
}

void Engine::PhysicsService::RemoveRigidBody(RigidBody2D* rb)
{
    for (auto it = m_RigidBodies.begin(); it != m_RigidBodies.end(); ++it)
    {
        if (*it == rb)
        {
            m_RigidBodies.erase(it);
            break;
        }
    }
}

void Engine::PhysicsService::Update()
{
    for (auto rb : m_RigidBodies)
    {
        for (auto other : m_RigidBodies)
        {
            // self ignore
            if (rb == other)
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