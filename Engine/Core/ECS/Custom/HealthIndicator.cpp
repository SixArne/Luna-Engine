#include "pch.h"
#include "HealthIndicator.h"
#include "GameObject.h"

#include "Core/Event/EventManager.h"
#include "Core/ECS/TextComponent.h"
#include "Core/ECS/Custom/HealthComponent.h"

Engine::HealthIndicator::HealthIndicator(GameObject* gameobject, int beginHealth)
	: TextComponent(gameobject)
{
	auto currentHealthText = std::format("{} lives left", beginHealth);
	SetText(currentHealthText);
}

void Engine::HealthIndicator::Init()
{
	TextComponent::Init();
}

void Engine::HealthIndicator::Update()
{
	TextComponent::Update();
}

void Engine::HealthIndicator::Render()
{
	TextComponent::Render();
}

void Engine::HealthIndicator::OnNotify(int data)
{
	auto currentHealthText = std::format("{} lives left", data);
	SetText(currentHealthText);
}