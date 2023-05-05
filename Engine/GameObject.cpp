#include "pch.h"

#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"

#include <ImGui.h>

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

void Engine::GameObject::AttachChild(std::shared_ptr<GameObject> child, bool keepWorldTransform)
{
	auto childTransform = child->GetTransform();

	if (keepWorldTransform)
	{
		childTransform->SetLocalPosition(childTransform->GetLocalPosition() - GetTransform()->GetWorldPosition());
		childTransform->SetPositionDirty();
	}
	else
	{
		childTransform->SetLocalPosition(childTransform->GetWorldPosition());
	}

	// If child is still attached to other object: detach
	if (auto parentOfChild = child->GetParent(); parentOfChild != nullptr)
	{
		parentOfChild->DetachChild(child);
	}

	if (child->m_Parent = this; child->m_Parent != nullptr)
	{
		AddChild(child);
	}
}

void Engine::GameObject::DetachChild(std::shared_ptr<GameObject> child)
{
	RemoveChild(child);
}

void Engine::GameObject::AddChild(std::shared_ptr<GameObject> child)
{
	m_Children.emplace_back(child);
}

void Engine::GameObject::RemoveChild(std::shared_ptr<GameObject> child)
{
	m_Children.erase(
		std::remove_if(m_Children.begin(), m_Children.end(), [child](auto fchild) { return fchild == child; })
	);
}

Engine::GameObject* Engine::GameObject::GetParent()
{
	return m_Parent;
}

std::vector<std::shared_ptr<Engine::GameObject>>& Engine::GameObject::GetChildren()
{
	return m_Children;
}

Engine::TransformComponent* Engine::GameObject::GetTransform()
{
	return m_TransformComponent;
}

void Engine::GameObject::Init()
{
	for (const auto& componentPair : m_Components)
	{
    	const auto& components = componentPair.second;

		for (const auto& c: components)
		{
			c->Init();
		}
	}

	for (auto& child : m_Children)
	{
		child->Init();
	}
}

void Engine::GameObject::Update()
{
	// Update own components
	for (const auto& componentPair : m_Components)
	{
    	const auto& components = componentPair.second;

		for (const auto& c: components)
		{
			c->Update();
		}
	}

	// Tell children to update themselves
	for (const auto& child : m_Children)
	{
		child->Update();
	}
}

void Engine::GameObject::LateUpdate()
{
	for (const auto& componentPair : m_Components)
	{
    	const auto& components = componentPair.second;

		for (const auto& c: components)
		{
			c->LateUpdate();
		}
	}

	for (const auto& child : m_Children)
	{
		child->LateUpdate();
	}
}

void Engine::GameObject::FixedUpdate(float fdt)
{
	for (const auto& componentPair : m_Components)
	{
    	const auto& components = componentPair.second;

		for (const auto& c: components)
		{
			c->FixedUpdate(fdt);
		}
	}

	for (const auto& child : m_Children)
	{
		child->FixedUpdate(fdt);
	}
}

void Engine::GameObject::Render() const
{
	for (const auto& componentPair : m_Components)
	{
    	const auto& components = componentPair.second;

		for (const auto& c: components)
		{
			c->Render();
		}
	}

	for (const auto& child : m_Children)
	{
		child->Render();
	}
}

void Engine::GameObject::OnImGui()
{
	if (m_RenderImGui)
	{
		ImGui::Begin(GetName().c_str());
		for (const auto& componentPair : m_Components)
		{
			const auto& components = componentPair.second;

			for (const auto& c: components)
			{
				c->OnImGui();
			}
		}
		ImGui::End();
	}

	for (const auto& child : m_Children)
	{
		child->OnImGui();
	}
}
