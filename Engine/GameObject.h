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
		void Init();
		void Update();
		void Render() const;

		GameObject(const char* name);
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
		bool HasComponent()
		{
			auto typeIdentifier = std::type_index(typeid(T));

			return m_Components.contains(typeIdentifier);
		}

		template<ComponentType T>
		void RemoveComponent()
		{
			const auto typeIdentifier = std::type_index(typeid(T));
			auto component = m_Components[typeIdentifier];

			delete component;

			m_Components.erase(typeIdentifier);
		}

		template<ComponentType T>
		void AddComponent(T* component)
		{
			// Generate the type identifier to find the component with.
			auto typeIdentifier = std::type_index(typeid(T));

			// Add self reference to component
			component->SetOwner(this);
			component->ComponentAttach();

			// Save component in map
			m_Components.emplace(typeIdentifier, component);

			L_INFO("{} added to GameObject", typeIdentifier.name())
		}

		const std::string& GetName();

	private:
		std::unordered_map<std::type_index, Component*> m_Components{};
		std::string m_GameObjectName{};
	};
}
