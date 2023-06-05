#include "pch.h"

#include <SDL.h>

#include "Texture2D.h"
#include "TextureRenderer.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "GameObject.h"
#include "TransformComponent.h"

#include <ImGui.h>


Engine::TextureRenderer::TextureRenderer(GameObject* object)
	: Component{object}
{
}

Engine::TextureRenderer::TextureRenderer(GameObject* object, const std::string& textureFile)
	: Component{ object }, m_Texture{ ResourceManager::GetInstance().LoadTexture(textureFile) }
{
}

Engine::TextureRenderer::TextureRenderer(GameObject* object, std::shared_ptr<Texture2D> texture)
	: Component{ object }, m_Texture{ texture }
{
}

Engine::TextureRenderer::~TextureRenderer()
{
}

void Engine::TextureRenderer::Init()
{
	const auto gameObject = GetOwner();

	if (!gameObject->HasComponent<TransformComponent>())
	{
		L_ERROR("[{}] TextComponent requires [TransformComponent] to be attached on the same GameObject.", GetOwner()->GetName())
	}
}

void Engine::TextureRenderer::Update()
{

}

void Engine::TextureRenderer::Render()
{
	if (m_Texture == nullptr) return;

	GameObject* owner = GetOwner();
	if (owner == nullptr) return;

	auto transformComponent = owner->GetTransform();
	if (transformComponent == nullptr) return;

	glm::vec2 pos = transformComponent->GetWorldPosition();

	Renderer::GetInstance().RenderTexture(*m_Texture, pos.x, pos.y);
}

void Engine::TextureRenderer::SetTexture(std::string source)
{
	m_Texture = ResourceManager::GetInstance().LoadTexture(source);
}

void Engine::TextureRenderer::SetTexture(std::shared_ptr<Texture2D> texture)
{
	m_Texture = texture;
}

void Engine::TextureRenderer::SetEmptyTexture()
{
	m_Texture = nullptr;
}

void Engine::TextureRenderer::OnImGui()
{
	ImGui::Text("TextureComponent");
}

glm::ivec2 Engine::TextureRenderer::GetScreenSize()
{
	return m_Texture->GetSize();
}