#pragma once


#include "pch.h"
#include "Core/ECS/Component.h"
#include "Core/ECS/TransformComponent.h"

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

		GameObject(const char* name, glm::vec2 position = glm::vec2{}, float angle = 0);

		~GameObject();
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

		template<ComponentType T>
		T* GetComponent();

		template<ComponentType T>
		bool HasComponent() const;

		template<ComponentType T>
		void RemoveComponent();

		template<ComponentType T, typename...Args>
		T* AddComponent(Args&&... args);

		template<ComponentType T>
		T* AddComponent();

		const std::string& GetName();
		void SetShouldRenderImGui(bool value) { m_RenderImGui = value; };

		GameObject* GetParent();
		std::vector<std::shared_ptr<GameObject>>& GetChildren();

		TransformComponent* GetTransform();

		void AttachChild(std::shared_ptr<GameObject> child, bool keepWorldTransform);
		void DetachChild(std::shared_ptr<GameObject> child);

	private:
		void AddChild(std::shared_ptr<GameObject> child);
		void RemoveChild(std::shared_ptr<GameObject> child);

		std::unordered_map<std::type_index, std::unique_ptr<Component>> m_Components{};
		TransformComponent* m_TransformComponent{};

		std::vector<std::shared_ptr<GameObject>> m_Children{};
		GameObject* m_Parent{};
		bool m_RenderImGui{false};
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

		auto componentData = static_cast<T*>(m_Components[typeIdentifier].get());

		return componentData;
	}

	template <ComponentType T>
	bool GameObject::HasComponent() const
	{
		// Generate the type identifier to find the component with.
		const auto typeIdentifier = std::type_index(typeid(T));

		// Used for dependency validation
		return m_Components.contains(typeIdentifier);
	}

	template <ComponentType T>
	void GameObject::RemoveComponent()
	{
		// Generate the type identifier to find the component with.
		const auto typeIdentifier = std::type_index(typeid(T));

		// Fetch component
		const auto component = m_Components[typeIdentifier].get();

		if (component->GetCanBeRemoved())
		{
			// Mark for deletion in lateUpdate
			component->MarkForDeletion();
			L_TRACE("{} marked for deletion obj: [{}]", typeIdentifier.name(), m_GameObjectName)
		}
		else
		{
			L_ERROR("{} can not be removed from [{}] as it is marked as NOT REMOVABLE.", typeIdentifier.name(), m_GameObjectName);
		}
	}

	template <ComponentType T, typename...Args>
	T* GameObject::AddComponent(Args&&...args)
	{
		// CREATE COMPONENT HERE INSTEAD
		// Generate the type identifier to find the component with.
		auto typeIdentifier = std::type_index(typeid(T));

		auto component = new T{this, std::forward<Args>(args)...};

		// Save component in map
		m_Components.emplace(typeIdentifier, component);
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
		m_Components.emplace(typeIdentifier, component);
		component->Attach();
		L_TRACE("{} added to: \t[{}]", typeIdentifier.name(), m_GameObjectName)
		return component;
	}

}
