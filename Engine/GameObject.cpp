#include "pch.h"

#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"

Engine::GameObject::GameObject(const char* name, glm::vec2 position, float angle)
	: m_GameObjectName{name}
{
	AddComponent<TransformComponent>(position, angle);
	m_TransformComponent = GetComponent<TransformComponent>();
}

Engine::GameObject::~GameObject()
{
	m_Components.clear();
}

const std::string& Engine::GameObject::GetName()
{
	return m_GameObjectName;
}

void Engine::GameObject::SetParent(GameObject* parent, bool keepWorldTransform)
{
	if (parent == nullptr)
	{
		m_TransformComponent->SetLocalPosition(m_TransformComponent->GetWorldPosition());
	}
	else
	{
		if (keepWorldTransform)
		{
			m_TransformComponent->SetLocalPosition(m_TransformComponent->GetLocalPosition() - parent->GetTransform()->GetWorldPosition());
			m_TransformComponent->SetPositionDirty();
		}
	}

	if (m_Parent)
	{
		m_Parent->RemoveChild(this);
	}

	m_Parent = parent;
	if (m_Parent)
	{
		parent->AddChild(this);
	}
}

void Engine::GameObject::AddChild(GameObject* child)
{
	m_Children.emplace_back(child);
}

void Engine::GameObject::RemoveChild(GameObject* child)
{
	m_Children.erase(
		std::remove_if(m_Children.begin(), m_Children.end(), [child](auto fchild) { return fchild == child; })
	);
}

Engine::GameObject* Engine::GameObject::GetParent()
{
	return m_Parent;
}

std::vector<Engine::GameObject*>& Engine::GameObject::GetChildren()
{
	return m_Children;
}

Engine::TransformComponent* Engine::GameObject::GetTransform()
{
	return m_TransformComponent;
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
