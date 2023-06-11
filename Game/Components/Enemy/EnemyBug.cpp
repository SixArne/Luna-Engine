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
#include <Core/Event/EventManager.h>
#include "Scene.h"

#include "Components/Util/AutoKill.h"


#define EVENT(name, level) std::format("{}{}", name, level)

Galaga::EnemyBug::EnemyBug(GameObject* object, glm::vec2 targetPosition)
    : BaseEnemy{ object }, m_TargetPosition{ targetPosition }
{
    m_ProjectileTexture = Engine::ResourceManager::GetInstance().LoadTexture("Resources/Sprites/projectile.png");
}

void Galaga::EnemyBug::Init()
{
    auto rb = GetOwner()->GetComponent<Engine::RigidBody2D>();
    auto animator = GetOwner()->GetComponent<Engine::SpriteAnimator>();

    rb->SetOnCollisionEnterCallback([this, animator](Engine::RigidBody2D* other)
        {
            if (m_IsDead) return;

            if (other->GetOwner()->HasTag("bullet"))
            {
                animator->SetState("death");
                auto ss = Engine::ServiceLocator::GetSoundService();
                ss->Play("Resources/Audio/shoot_short.wav");

                // Destroy bullet
                other->GetOwner()->Destroy();
                L_DEBUG("Bullet hit");


                m_IsDead = true;

                Engine::EventManager::GetInstance().Notify("EnemyDead", nullptr);
                Engine::EventManager::GetInstance().Notify(EVENT("BeeDiedDiving", GetOwner()->GetScene()->GetName()), nullptr);
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
