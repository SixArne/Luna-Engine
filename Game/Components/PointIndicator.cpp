#include "pch.h"
#include "PointIndicator.h"

#include <GameObject.h>
#include "Core/Event/EventManager.h"
#include "Core/ECS/TextComponent.h"

Galaga::PointIndicator::PointIndicator(GameObject* gameobject, int /*beginPoints*/)
	:Component{ gameobject }
{
}

void Galaga::PointIndicator::Init()
{
	const auto gameObject = GetOwner();

	if (!gameObject->HasComponent<TextComponent>())
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

void Galaga::PointIndicator::Update()
{
}

void Galaga::PointIndicator::Render()
{
}

void Galaga::PointIndicator::OnNotify(int data)
{
	auto currentPointsText = std::format("{} points", data);
	m_TextComponent->SetText(currentPointsText);
}