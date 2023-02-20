#include "pch.h"

#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"

Engine::GameObject::~GameObject() = default;

void Engine::GameObject::Update(){}

void Engine::GameObject::Render() const
{
	//const auto& pos = m_transform.GetPosition();
	//Renderer::GetInstance().RenderTexture(*m_texture, pos.x, pos.y);
}

//void Engine::GameObject::SetTexture(const std::string& filename)
//{
//	m_texture = ResourceManager::GetInstance().LoadTexture(filename);
//}
//
//void Engine::GameObject::SetPosition(float x, float y)
//{
//	m_transform.SetPosition(x, y, 0.0f);
//}
