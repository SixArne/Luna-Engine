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

Galaga::SpaceFighter::SpaceFighter(GameObject* object)
    : Component{ object }
{
    m_ProjectileTexture = Engine::ResourceManager::GetInstance().LoadTexture("Resources/Sprites/projectile.png");
}

void Galaga::SpaceFighter::Init()
{
}

void Galaga::SpaceFighter::Update()
{
}

void Galaga::SpaceFighter::Render()
{
}

void Galaga::SpaceFighter::Shoot()
{
    auto currentLocation = GetOwner()->GetTransform()->GetWorldPosition();

    auto projectile = std::make_shared<Engine::GameObject>("projectile", currentLocation);
    projectile->AddComponent<Engine::TextureRendererComponent>(m_ProjectileTexture);
    projectile->AddComponent<Galaga::Projectile>(glm::vec2{ 0.f, 1.f }, glm::vec2{ 0.f, -100.f });
    projectile->AddComponent<Galaga::AutoKill>(5.f);

    Engine::SceneManager::GetInstance().GetScene("Game")->Instantiate(projectile);

    auto ss = Engine::ServiceLocator::GetSoundService();
    ss->Play("Resources/Audio/shoot_short.wav", 0.5f);
}
