#include "pch.h"
#include "HealthComponent.h"

Engine::HealthComponent::HealthComponent(GameObject* gameobject)
	:Component{gameobject}
{
}

Engine::HealthComponent::~HealthComponent()
{
}

void Engine::HealthComponent::Init()
{
}

void Engine::HealthComponent::Update()
{
}

void Engine::HealthComponent::Render()
{
}

void Engine::HealthComponent::SetHealth(int health)
{
	m_Health = health;
}

int Engine::HealthComponent::GetHealth() const
{
	return m_Health;
}

void Engine::HealthComponent::TakeDamage(int damage)
{
	m_Health -= damage;
	m_Health = std::abs(m_Health);
}
