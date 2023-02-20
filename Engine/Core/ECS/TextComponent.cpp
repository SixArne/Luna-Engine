#include "pch.h"
#include "TextComponent.h"

Engine::TextComponent::TextComponent(std::string text)
	:m_TextToRender{text}
{}

void Engine::TextComponent::ComponentUpdate()
{
}

void Engine::TextComponent::ComponentRender()
{
}

std::string Engine::TextComponent::GetText()
{
	return m_TextToRender;
}

void Engine::TextComponent::SetText(std::string text)
{
	m_TextToRender = text;
}
