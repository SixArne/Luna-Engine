#pragma once


#include "pch.h"
#include "Core/ECS/Component.h"
#include "Core/ECS/TransformComponent.h"

#include <set>

// Component only guard
template<typename T>
concept ComponentType = std::is_base_of<Engine::Component, T>::value;

namespace Engine
{
	class Texture2D;

	class GameObject final
	{
	public:
		void Init();
		void Update();
		void LateUpdate();
		void FixedUpdate(float fdt);
		void Render() const;
		void OnImGui();
		void OnSceneLoad();
		void OnSceneUnload();

		GameObject(const char* name, glm::vec2 position = glm::vec2{}, float angle = 0);

		~GameObject();
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

		template<ComponentType T>
		T* GetComponent();

		template<ComponentType T>
		std::vector<T*> GetComponents();

		template<ComponentType T>
		bool HasComponent() const;

		template<ComponentType T, typename...Args>
		T* AddComponent(Args&&... args);

		template<ComponentType T>
		T* AddComponent();

		const std::string& GetName();
		void SetShouldRenderImGui(bool value) { m_RenderImGui = value; };

		void SetActive(bool value);
		bool IsActive() const;
		bool IsMarkedForDeletion() const;
		void MarkForDeletion();

		GameObject* GetParent();
		std::vector<std::shared_ptr<GameObject>>& GetChildren();

		TransformComponent* GetTransform();

		void AttachChild(std::shared_ptr<GameObject> child, bool keepWorldTransform);
		void DetachChild(std::shared_ptr<GameObject> child);

		void SetCanBeDestroyed(bool value);
		bool CanBeDestroyed() const;
		bool HasTag(const std::string& tag);
		void AddTag(const std::string& tag);

		void Destroy();

	private:
		void AddChild(std::shared_ptr<GameObject> child);
		void RemoveChild(std::shared_ptr<GameObject> child);

		using ComponentContainer = std::unordered_map<std::type_index, std::vector<std::unique_ptr<Component>>>;
		ComponentContainer m_Components{};
		TransformComponent* m_TransformComponent{};

		std::set<std::string> m_Tags{};
		std::vector<std::shared_ptr<GameObject>> m_Children{};
		GameObject* m_Parent{};

		bool m_RenderImGui{false};
		bool m_IsActive{true};
		bool m_MarkedForDeletion{false};
		bool m_CanBeDestroyed{true};

		std::string m_GameObjectName{};
	};

	template <ComponentType T>
	T* GameObject::GetComponent()
	{
		// Generate the type identifier to find the component with.
		const auto typeIdentifier = std::type_index(typeid(T));
		if (!m_Components.contains(typeIdentifier))
		{
			return nullptr;
		}

		if (m_Components[typeIdentifier].size() > 0)
		{
			auto componentData = static_cast<T*>(m_Components[typeIdentifier][0].get());
			return componentData;
		}
		else
		{
			return nullptr;
		}
	}

	template <ComponentType T>
	std::vector<T*> GameObject::GetComponents()
	{
		std::vector<T*> components{};

		// Generate the type identifier to find the component with.
		const auto typeIdentifier = std::type_index(typeid(T));
		if (!m_Components.contains(typeIdentifier))
		{
			return nullptr;
		}

		if (m_Components[typeIdentifier].size() > 0)
		{
			for (const auto &component : m_Components[typeIdentifier])
			{
				components.push_back(static_cast<T*>(component.get()));
			}
		}

		return components;
	}

	template <ComponentType T>
	bool GameObject::HasComponent() const
	{
		// Generate the type identifier to find the component with.
		const auto typeIdentifier = std::type_index(typeid(T));

		// Used for dependency validation
		return m_Components.contains(typeIdentifier);
	}

	template <ComponentType T, typename...Args>
	T* GameObject::AddComponent(Args&&...args)
	{
		// CREATE COMPONENT HERE INSTEAD
		// Generate the type identifier to find the component with.
		auto typeIdentifier = std::type_index(typeid(T));

		auto component = new T{this, std::forward<Args>(args)...};

		// Save component in map
		if (!m_Components.contains(typeIdentifier))
		{
			m_Components[typeIdentifier] = std::vector<std::unique_ptr<Component>>();
		}

		m_Components[typeIdentifier].push_back(std::unique_ptr<Component>(component));

		component->Attach();

		L_TRACE("{} added to: \t[{}]", typeIdentifier.name(), m_GameObjectName)
		return component;
	}

	template<ComponentType T>
	T* GameObject::AddComponent()
	{
		// CREATE COMPONENT HERE INSTEAD
		// Generate the type identifier to find the component with.
		auto typeIdentifier = std::type_index(typeid(T));

		auto component = new T{ this };

		// Save component in map
		if (!m_Components.contains(typeIdentifier))
		{
			m_Components[typeIdentifier] = std::vector<std::unique_ptr<Component>>();
		}

		m_Components[typeIdentifier].push_back(std::unique_ptr<Component>(component));

		component->Attach();

		L_TRACE("{} added to: \t[{}]", typeIdentifier.name(), m_GameObjectName)
		return component;
	}
}
