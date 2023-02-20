#include "pch.h"

#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"

Engine::GameObject::~GameObject() = default;

void Engine::GameObject::Update()
{
	for (const auto& component : m_Components)
	{
		component.second->ComponentUpdate();
	}
}

void Engine::GameObject::Render() const
{
	for (const auto& component : m_Components)
	{
		component.second->ComponentRender();
	}
}
