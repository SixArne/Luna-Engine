#include "pch.h"
#include "TextComponent.h"
#include <SDL_ttf.h>

#include "Font.h"
#include "GameObject.h"
#include "Renderer.h"
#include "Texture2D.h"
#include "TransformComponent.h"
#include "ResourceManager.h"

Engine::TextComponent::TextComponent(GameObject* object)
	: Component{object}, m_Font{Engine::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36)}
{
	RenderText();
}

Engine::TextComponent::TextComponent(GameObject* object, std::string text)
	: Component{ object }, m_TextToRender{text}, m_Font{ Engine::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36) }
{
	RenderText();
}

Engine::TextComponent::TextComponent(GameObject* object, std::string text, glm::vec2 offset)
	: Component{ object }
	, m_TextToRender{text}
	, m_Font{ Engine::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36) }
	, m_Offset{offset}
{
	RenderText();
}

Engine::TextComponent::TextComponent(GameObject* object, std::string text, std::shared_ptr<Font> font)
	: Component{object}, m_TextToRender{text}, m_Font{font}
{
	RenderText();
}

void Engine::TextComponent::Init()
{
	const auto gameObject = GetOwner();

	if (!gameObject->HasComponent<TransformComponent>())
	{
		L_ERROR("[{}] TextComponent requires [TransformComponent] to be attached on the same GameObject.", GetOwner()->GetName())
	}
}

void Engine::TextComponent::Update()
{
	if (m_MarkedForUpdate)
	{
		RenderText();
	}
}

void Engine::TextComponent::Render()
{
	if (m_TextTexture != nullptr)
	{
		const auto& transformComponent = GetOwner()->GetTransform();
		if (transformComponent != nullptr)
		{
			auto position = transformComponent->GetWorldPosition();

			Renderer::GetInstance().RenderTexture(*m_TextTexture, position.x + m_Offset.x, position.y + m_Offset.y);
		}
	}
}

std::string Engine::TextComponent::GetText()
{
	return m_TextToRender;
}

void Engine::TextComponent::SetText(std::string text)
{
	m_TextToRender = text;
	m_MarkedForUpdate = true;
}

void Engine::TextComponent::SetFont(std::shared_ptr<Font> font)
{
	m_Font = font;
	m_MarkedForUpdate = true;
}

glm::ivec2 Engine::TextComponent::GetScreenSize()
{
	return m_TextTexture->GetSize();
}

void Engine::TextComponent::SetOffset(glm::vec2 offset)
{
	m_Offset = offset;
	m_MarkedForUpdate = true;
}

void Engine::TextComponent::SetColor(const glm::ivec3& color)
{
	m_Color = color;
	m_MarkedForUpdate = true;
}

void Engine::TextComponent::RenderText()
{
	const SDL_Color color = { (Uint8)m_Color.r, (Uint8)m_Color.g, (Uint8)m_Color.b };
	const auto surface = TTF_RenderText_Blended(m_Font->GetFont(), m_TextToRender.c_str(), color);
	if (surface == nullptr)
	{
		L_ERROR("Render text failed: {}", SDL_GetError())
	}

	auto texture = SDL_CreateTextureFromSurface(Renderer::GetInstance().GetSDLRenderer(), surface);
	if (texture == nullptr)
	{
		L_ERROR("Create text texture from surface failed: {}", SDL_GetError())
	}

	SDL_FreeSurface(surface);
	m_TextTexture = std::make_shared<Texture2D>(texture);
	m_MarkedForUpdate = false;
}