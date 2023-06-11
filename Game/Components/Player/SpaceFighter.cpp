#include "SpaceFighter.h"
#include <SceneManager.h>
#include <Core/ECS/TextureRenderer.h>
#include <GameObject.h>
#include <Scene.h>
#include "Components/Util/AutoKill.h"
#include "Components/Player/Projectile.h"
#include "Components/Enemy/BaseEnemy.h"
#include <ResourceManager.h>

#include <Core/Services/ServiceLocator.h>
#include <Core/Services/Sound/ISoundService.h>
#include <Core/ECS/RigidBody2D.h>
#include <Core/Log.h>
#include <Core/Time.h>

#include <Core/Event/EventManager.h>

#include <algorithm>

Galaga::SpaceFighter::SpaceFighter(GameObject* object)
    : Component{ object }
{
    m_ProjectileTexture = Engine::ResourceManager::GetInstance().LoadTexture("Resources/Sprites/projectile.png");
}

void Galaga::SpaceFighter::Init()
{
    auto rb = GetOwner()->GetComponent<Engine::RigidBody2D>();

    rb->SetOnCollisionEnterCallback([this](Engine::RigidBody2D* other)
        {
            auto enemyComp = other->GetOwner()->GetComponentDerivedFrom<Galaga::BaseEnemy>();

            if (enemyComp == nullptr)
            {
                L_ERROR("Error when fetching baseEnemyComponent");
            }

            if (other->GetOwner()->HasTag("enemy") && enemyComp->HasFinishedIntroFlight())
            {
                m_Lives = std::clamp(m_Lives - 1, 0, 3);
                NotifyObservers(m_Lives);
            }
        });

    rb->SetOnCollisionExitCallback([this](Engine::RigidBody2D* )
        {
        });

    rb->SetOnCollisionCallback([this](Engine::RigidBody2D*)
        {
        });
}

void Galaga::SpaceFighter::Update()
{
    if (m_ShootCooldownTimer <= m_ShootCooldown)
    {
        m_ShootCooldownTimer += Engine::Time::GetInstance().GetDeltaTime();
    }

    if (m_Lives == 0)
    {
	    Engine::EventManager::GetInstance().Notify("EndGame", nullptr);

        GetOwner()->Destroy();
        Engine::SceneManager::GetInstance().SwitchToScene("menu");
    }
}

void Galaga::SpaceFighter::Render()
{
}

void Galaga::SpaceFighter::Shoot()
{
    if (m_ShootCooldownTimer < m_ShootCooldown)
    {
        return;
    }

    auto currentLocation = GetOwner()->GetTransform()->GetWorldPosition();

    auto projectile = std::make_shared<Engine::GameObject>("projectile", currentLocation);
    projectile->AddComponent<Engine::TextureRenderer>(m_ProjectileTexture);
    projectile->AddComponent<Galaga::Projectile>(glm::vec2{ 0.f, 1.f }, glm::vec2{ 0.f, -200.f });
    projectile->AddComponent<Galaga::AutoKill>(15.f);
    projectile->AddComponent<Engine::RigidBody2D>(Engine::RigidBody2DCollider{5,5});
    projectile->AddTag("bullet");

    Engine::SceneManager::GetInstance().GetActiveScene()->Instantiate(projectile);
    L_DEBUG("Adding bullet");

    auto ss = Engine::ServiceLocator::GetSoundService();
    ss->Play("Resources/Audio/shoot_short.wav");

    m_ShootCooldownTimer = 0.f;
}
