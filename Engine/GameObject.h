#pragma once


#include "pch.h"
#include "Core/ECS/Component.h"

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

		GameObject(const char* name);
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
		void AddComponent(Args&&... args);

		template<ComponentType T>
		void AddComponent();

		const std::string& GetName();

	private:
		std::unordered_map<std::type_index, Component*> m_Components{};
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

		auto componentData = reinterpret_cast<T*>(m_Components[typeIdentifier]);

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
		const auto component = m_Components[typeIdentifier];

		// Mark for deletion in lateUpdate
		component->MarkForDeletion();

		L_TRACE("{} marked for deletion obj: [{}]", typeIdentifier.name(), m_GameObjectName)
	}

	template <ComponentType T, typename...Args>
	void GameObject::AddComponent(Args&&...args)
	{
		// CREATE COMPONENT HERE INSTEAD
		// Generate the type identifier to find the component with.
		auto typeIdentifier = std::type_index(typeid(T));

		auto component = new T{this, std::forward<Args>(args)...};

		// Save component in map
		m_Components.emplace(typeIdentifier, component);
		component->ComponentAttach();
		L_TRACE("{} added to: \t[{}]", typeIdentifier.name(), m_GameObjectName)
	}

	template<ComponentType T>
	void GameObject::AddComponent()
	{
		// CREATE COMPONENT HERE INSTEAD
		// Generate the type identifier to find the component with.
		auto typeIdentifier = std::type_index(typeid(T));

		auto component = new T{ this };

		// Save component in map
		m_Components.emplace(typeIdentifier, component);
		component->ComponentAttach();
		L_TRACE("{} added to: \t[{}]", typeIdentifier.name(), m_GameObjectName)
	}

}
