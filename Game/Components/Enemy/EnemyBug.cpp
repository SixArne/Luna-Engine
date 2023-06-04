#include "pch.h"
#include "EnemyBug.h"
#include <Core/ECS/TextureRenderer.h>
#include <GameObject.h>
#include <ResourceManager.h>
#include <Core/ECS/RigidBody2D.h>
#include <Core/ECS/SpriteAnimator.h>
#include <Core/Services/ServiceLocator.h>
#include <Core/Services/Sound/ISoundService.h>

#include <Core/Log.h>
#include <Core/Time.h>

Galaga::EnemyBug::EnemyBug(GameObject* object)
    : Component{ object }
{
    m_ProjectileTexture = Engine::ResourceManager::GetInstance().LoadTexture("Resources/Sprites/projectile.png");
}

void Galaga::EnemyBug::Init()
{
    auto rb = GetOwner()->GetComponent<Engine::RigidBody2D>();
    auto animator = GetOwner()->GetComponent<Engine::SpriteAnimator>();

    rb->SetOnCollisionEnterCallback([this, animator](Engine::RigidBody2D* other)
        {
            if (other->GetOwner()->HasTag("bullet"))
            {
                //animator->SetState("death");
                auto ss = Engine::ServiceLocator::GetSoundService();
                ss->Play("Resources/Audio/shoot_short.wav", 0.5f);
            }
        });

    rb->SetOnCollisionExitCallback([this](Engine::RigidBody2D*)
        {
        });

    rb->SetOnCollisionCallback([this](Engine::RigidBody2D*)
        {
        });
}

void Galaga::EnemyBug::Update()
{
    if (m_ShootCooldownTimer <= m_ShootCooldown)
    {
        m_ShootCooldownTimer += Engine::Time::GetInstance().GetDeltaTime();
    }
}

void Galaga::EnemyBug::Render()
{
}
