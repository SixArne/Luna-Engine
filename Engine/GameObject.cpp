#include "pch.h"

#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"

Engine::GameObject::GameObject(const char* name)
	: m_GameObjectName{name}
{}

Engine::GameObject::~GameObject()
{
	for (const auto& component : m_Components)
	{
		delete component.second;
	}

	m_Components.clear();
}

const std::string& Engine::GameObject::GetName()
{
	return m_GameObjectName;
}

void Engine::GameObject::Init()
{
	for (const auto& component : m_Components)
	{
		component.second->ComponentInit();
	}
}

void Engine::GameObject::Update()
{
	for (const auto& component : m_Components)
	{
		component.second->ComponentUpdate();
	}
}

void Engine::GameObject::LateUpdate()
{
	for (const auto& component : m_Components)
	{
		if (!component.second->IsMarkedForDeletion())
		{
			component.second->ComponentLateUpdate();
		}
		else
		{
			// Clean up memory
			delete component.second;

			// Erase from component list
			m_Components.erase(component.first);
		}
	}
}

void Engine::GameObject::FixedUpdate(float fdt)
{
	for (const auto& component : m_Components)
	{
		component.second->ComponentFixedUpdate(fdt);
	}
}

void Engine::GameObject::Render() const
{
	for (const auto& component : m_Components)
	{
		component.second->ComponentRender();
	}
}
