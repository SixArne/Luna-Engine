#include "pch.h"

#include <SDL.h>

#include "Texture2D.h"
#include "TextureRenderer.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "GameObject.h"
#include "TransformComponent.h"

Engine::TextureRendererComponent::TextureRendererComponent()
	:m_Texture{ nullptr }
{
}

Engine::TextureRendererComponent::~TextureRendererComponent()
{
}

void Engine::TextureRendererComponent::ComponentUpdate()
{
	
}

void Engine::TextureRendererComponent::ComponentRender()
{
	if (m_Texture == nullptr) return;

	GameObject* owner = GetOwner();
	if (owner == nullptr) return;

	auto transformComponent = owner->GetComponent<Engine::TransformComponent>();
	if (transformComponent == nullptr) return;

	glm::vec2 pos = transformComponent->GetPosition();
	Renderer::GetInstance().RenderTexture(*m_Texture, pos.x, pos.y);
}

void Engine::TextureRendererComponent::SetTexture(std::string source)
{
	m_Texture = ResourceManager::GetInstance().LoadTexture(source);
}

void Engine::TextureRendererComponent::ComponentInit()
{
	const auto gameObject = GetOwner();

	if (!gameObject->HasComponent<TransformComponent>())
	{
		L_ERROR("TextComponent requires [TransformComponent] to be attached on the same GameObject.")
	}
}