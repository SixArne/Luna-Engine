#include "RigidBody2D.h"
#include "Core/Services/ServiceLocator.h"

void Engine::RigidBody2D::Init()
{
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