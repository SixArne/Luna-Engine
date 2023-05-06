#include "pch.h"

#include <SDL.h>

#include "Texture2D.h"
#include "TextureRenderer.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "GameObject.h"
#include "TransformComponent.h"

#include <ImGui.h>


Engine::TextureRendererComponent::TextureRendererComponent(GameObject* object)
	: Component{object}
{
}

Engine::TextureRendererComponent::TextureRendererComponent(GameObject* object, const std::string& textureFile)
	: Component{ object }, m_Texture{ ResourceManager::GetInstance().LoadTexture(textureFile) }
{
}

Engine::TextureRendererComponent::TextureRendererComponent(GameObject* object, std::shared_ptr<Texture2D> texture)
	: Component{ object }, m_Texture{ texture }
{
}

Engine::TextureRendererComponent::~TextureRendererComponent()
{
}

void Engine::TextureRendererComponent::Init()
{
	const auto gameObject = GetOwner();

	if (!gameObject->HasComponent<TransformComponent>())
	{
		L_ERROR("[{}] TextComponent requires [TransformComponent] to be attached on the same GameObject.", GetOwner()->GetName())
	}
}

void Engine::TextureRendererComponent::Update()
{

}

void Engine::TextureRendererComponent::Render()
{
	if (m_Texture == nullptr) return;

	GameObject* owner = GetOwner();
	if (owner == nullptr) return;

	auto transformComponent = owner->GetTransform();
	if (transformComponent == nullptr) return;

	glm::vec2 pos = transformComponent->GetWorldPosition();

	Renderer::GetInstance().RenderTexture(*m_Texture, pos.x, pos.y);
}

void Engine::TextureRendererComponent::SetTexture(std::string source)
{
	m_Texture = ResourceManager::GetInstance().LoadTexture(source);
}

void Engine::TextureRendererComponent::OnImGui()
{
	ImGui::Text("TextureComponent");
}
