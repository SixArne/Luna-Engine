#include "pch.h"
#include "SpaceFighter.h"
#include <SceneManager.h>
#include <Core/ECS/TextureRenderer.h>
#include <GameObject.h>
#include <Scene.h>
#include "Components/Util/AutoKill.h"
#include "Components/Player/Projectile.h"
#include <ResourceManager.h>

#include <Core/Services/ServiceLocator.h>
#include <Core/Services/Sound/ISoundService.h>
#include <Core/ECS/RigidBody2D.h>
#include <Core/Log.h>
#include <Core/Time.h>

Galaga::SpaceFighter::SpaceFighter(GameObject* object)
    : Component{ object }
{
    m_ProjectileTexture = Engine::ResourceManager::GetInstance().LoadTexture("Resources/Sprites/projectile.png");
}

void Galaga::SpaceFighter::Init()
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

void Galaga::SpaceFighter::Update()
{
    if (m_ShootCooldownTimer <= m_ShootCooldown)
    {
        m_ShootCooldownTimer += Engine::Time::GetInstance().GetDeltaTime();
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
    projectile->AddComponent<Engine::TextureRendererComponent>(m_ProjectileTexture);
    projectile->AddComponent<Galaga::Projectile>(glm::vec2{ 0.f, 1.f }, glm::vec2{ 0.f, -200.f });
    projectile->AddComponent<Galaga::AutoKill>(3.f);
    projectile->AddComponent<Engine::RigidBody2D>(Engine::RigidBody2DCollider{5,5});

    Engine::SceneManager::GetInstance().GetScene("Game")->Instantiate(projectile);

    auto ss = Engine::ServiceLocator::GetSoundService();
    ss->Play("Resources/Audio/shoot_short.wav", 0.5f);

    m_ShootCooldownTimer = 0.f;
}
