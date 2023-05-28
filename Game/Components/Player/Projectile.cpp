#include "pch.h"
#include "Projectile.h"

#include <GameObject.h>
#include <Core/Time.h>
#include <glm/vec2.hpp>
#include <Core/ECS/RigidBody2D.h>

Galaga::Projectile::Projectile(GameObject* object, glm::vec2 direction, glm::vec2 speed)
    : Component{ object }, m_Direction{ direction }, m_Speed{ speed }
{
}

void Galaga::Projectile::Init()
{
    auto rb = GetOwner()->GetComponent<Engine::RigidBody2D>();

     rb->SetOnCollisionEnterCallback([this](Engine::RigidBody2D* )
        {
        });

    rb->SetOnCollisionExitCallback([this](Engine::RigidBody2D* )
        {
        });

    rb->SetOnCollisionCallback([this](Engine::RigidBody2D*)
        {
        });
}

void Galaga::Projectile::Update()
{
    GetOwner()->GetTransform()->AddLocalPosition({ m_Speed * Engine::TIME.GetDeltaTime()});
}

void Galaga::Projectile::Render()
{
}
