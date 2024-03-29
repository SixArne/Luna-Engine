#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "Scene.h"

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
		if (child->IsActive())
		{
			child->Update();
		}
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
		if (child->IsActive())
		{
			child->LateUpdate();
		}
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
		if (child->IsActive())
		{
			child->FixedUpdate(fdt);
		};
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
		if (child->IsActive())
		{
			child->Render();
		}
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
		if (child->IsActive())
		{
			child->OnImGui();
		}
	}
}

void Engine::GameObject::OnSceneLoad()
{
	for (const auto& componentPair : m_Components)
	{
    	const auto& components = componentPair.second;

		for (const auto& c: components)
		{
			c->OnSceneLoad();
		}
	}

	for (const auto& child : m_Children)
	{
		if (child->IsActive())
		{
			child->OnSceneLoad();
		}
	}
}

void Engine::GameObject::OnSceneUnload()
{
	for (const auto& componentPair : m_Components)
	{
    	const auto& components = componentPair.second;

		for (const auto& c: components)
		{
			c->OnSceneUnload();
		}
	}

	for (const auto& child : m_Children)
	{
		if (child->IsActive())
		{
			child->OnSceneUnload();
		}
	}
}

void Engine::GameObject::SetActive(bool value)
{
	m_IsActive = value;
}

bool Engine::GameObject::IsActive() const
{
	return m_IsActive;
}

void Engine::GameObject::Destroy()
{
	m_MarkedForDeletion = true;
}

bool Engine::GameObject::IsMarkedForDeletion() const
{
	return m_MarkedForDeletion;
}

void Engine::GameObject::SetCanBeDestroyed(bool value)
{
	m_CanBeDestroyed = value;
}

bool Engine::GameObject::CanBeDestroyed() const
{
	return m_CanBeDestroyed;
}

bool Engine::GameObject::HasTag(const std::string& tag)
{
	return m_Tags.contains(tag);
}

void Engine::GameObject::AddTag(const std::string& tag)
{
	m_Tags.insert(std::string(tag));
}

void Engine::GameObject::MarkForDeletion()
{
	m_MarkedForDeletion = true;
}