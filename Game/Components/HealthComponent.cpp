#include "pch.h"
#include "HealthComponent.h"

Galaga::HealthComponent::HealthComponent(GameObject* gameobject)
	:Component{gameobject}
{
}

Galaga::HealthComponent::~HealthComponent()
{
}

void Galaga::HealthComponent::Init()
{
}

void Galaga::HealthComponent::Update()
{
}

void Galaga::HealthComponent::Render()
{
}

void Galaga::HealthComponent::SetHealth(int health)
{
	m_Health = health;
}

int Galaga::HealthComponent::GetHealth() const
{
	return m_Health;
}

void Galaga::HealthComponent::TakeDamage(int damage)
{
	m_Health -= damage;
	m_Health = std::abs(m_Health);

	NotifyObservers(m_Health);
}
