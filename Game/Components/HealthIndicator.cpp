#include "HealthIndicator.h"

#include <GameObject.h>
#include <Core/Event/EventManager.h>
#include <Core/ECS/TextComponent.h>

#include <iostream>

Galaga::HealthIndicator::HealthIndicator(GameObject* gameobject, int /*beginHealth*/)
	:Component{ gameobject }
{

}

void Galaga::HealthIndicator::Init()
{
	const auto gameObject = GetOwner();

	bool hasTextComponent = gameObject->HasComponent<TextComponent>();

	if (!hasTextComponent)
	{
		L_ERROR("[{}] FPSCounter requires [TextComponent] to be attached on the same GameObject.", GetOwner()->GetName())
	}
	else
	{
		m_TextComponent = gameObject->GetComponent<TextComponent>();
		auto currentHealthText = std::format("{} lives left", 0);
		m_TextComponent->SetText(currentHealthText);
	}
}

void Galaga::HealthIndicator::Update()
{
}

void Galaga::HealthIndicator::Render()
{
}

void Galaga::HealthIndicator::OnNotify(int data)
{
	auto currentHealthText = std::format("{} lives left", data);
	m_TextComponent->SetText(currentHealthText);
}