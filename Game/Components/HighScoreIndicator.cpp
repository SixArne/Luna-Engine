#include "pch.h"
#include "HighScoreIndicator.h"

#include <GameObject.h>
#include <Core/Event/EventManager.h>
#include <Core/ECS/TextComponent.h>

#include <iostream>

Galaga::HighscoreIndicator::HighscoreIndicator(GameObject* gameobject, int beginScore)
	:Component{ gameobject }, m_Score{ beginScore }
{

}

void Galaga::HighscoreIndicator::Init()
{
	const auto gameObject = GetOwner();

	bool hasTextComponent = gameObject->HasComponent<TextComponent>();

	if (!hasTextComponent)
	{
		L_ERROR("[{}] HighScoreIndicator requires [TextComponent] to be attached on the same GameObject.", GetOwner()->GetName())
	}
	else
	{
		m_TextComponent = gameObject->GetComponent<TextComponent>();
		auto currentHealthText = std::format("{}", m_Score);
		m_TextComponent->SetText(currentHealthText);
	}
}

void Galaga::HighscoreIndicator::Update()
{
}

void Galaga::HighscoreIndicator::Render()
{
}

void Galaga::HighscoreIndicator::OnNotify(int data)
{
	auto currentHealthText = std::format("{} lives left", data);
	m_TextComponent->SetText(currentHealthText);
}