#include "RigidBody2D.h"
#include "Core/Services/ServiceLocator.h"

uint64_t Engine::RigidBody2D::s_EntityID = 0;

void Engine::RigidBody2D::Init()
{
    m_EntityID = s_EntityID++; // Will be unique due to static

    ServiceLocator::GetPhysicsService()->AddRigidBody(this);
    // Needs to be set to avoid automatic cleanup. as references are being
    // used in seperate collision thread;
    GetOwner()->SetCanBeDestroyed(false);
}

void Engine::RigidBody2D::Remove()
{
}

Engine::RigidBody2D::~RigidBody2D()
{
}

void Engine::RigidBody2D::OnSceneLoad()
{
}

void Engine::RigidBody2D::OnSceneUnload()
{

}