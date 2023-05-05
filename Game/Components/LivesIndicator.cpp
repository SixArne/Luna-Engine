#include "pch.h"
#include "LivesIndicator.h"

#include <GameObject.h>
#include <Core/Event/EventManager.h>
#include <Core/ECS/TextComponent.h>

#include <iostream>

Galaga::LivesIndicator::LivesIndicator(GameObject* gameobject, int beginLives)
	:Component{ gameobject }
{

}

void Galaga::LivesIndicator::Init()
{
	const auto gameObject = GetOwner();

	bool hasTextComponent = gameObject->HasComponent<TextComponent>();

	if (!hasTextComponent)
	{
		L_ERROR("[{}] LivesIndicator requires [TextComponent] to be attached on the same GameObject.", GetOwner()->GetName())
	}
	else
	{
		m_TextComponent = gameObject->GetComponent<TextComponent>();
		auto currentHealthText = std::format("{} lives left", 0);
		m_TextComponent->SetText(currentHealthText);
	}
}

void Galaga::LivesIndicator::Update()
{
}

void Galaga::LivesIndicator::Render()
{
}

void Galaga::LivesIndicator::OnNotify(int data)
{
	auto currentHealthText = std::format("{} lives left", data);
	m_TextComponent->SetText(currentHealthText);
}