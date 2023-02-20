#pragma once


#include "pch.h"
#include "Transform.h"
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
		void Update();
		void Render() const;

		GameObject() = default;
		~GameObject();
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

		template<ComponentType T>
		T* GetComponent()
		{
			auto typeIdentifier = std::type_index(typeid(T));
			if (!m_Components.contains(typeIdentifier))
			{
				return nullptr;
			}

			auto componentData = reinterpret_cast<T*>(m_Components[typeIdentifier]);

			return componentData;
		}

		template<ComponentType T>
		void RemoveComponent()
		{
			m_Components.erase(std::type_index(typeid(T)));
		}

		template<ComponentType T>
		void AddComponent(T* component)
		{
			// Generate the type identifier to find the component with.
			auto typeIdentifier = std::type_index(typeid(T));

			// Add self reference to component
			component->SetOwner(this);

			// Save component in map
			m_Components.emplace(typeIdentifier, component);
		}

	private:
		std::unordered_map<std::type_index, Component*> m_Components{};
	};
}
