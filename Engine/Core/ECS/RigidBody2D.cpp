#include "RigidBody2D.h"
#include "Core/Services/ServiceLocator.h"

uint64_t Engine::RigidBody2D::s_EntityID = 0;

void Engine::RigidBody2D::Init()
{
    m_EntityID = s_EntityID++; // Will be unique due to static
    ServiceLocator::GetPhysicsService()->AddRigidBody(this);
}

void Engine::RigidBody2D::Remove()
{
    ServiceLocator::GetPhysicsService()->RemoveRigidBody(this);
}

Engine::RigidBody2D::~RigidBody2D()
{
    Remove();
}