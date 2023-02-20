#pragma once


#include "pch.h"
#include "Transform.h"
#include "Component.h"


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

		//void SetTexture(const std::string& filename);
		//void SetPosition(float x, float y);

		GameObject() = default;
		~GameObject();
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

	/*	template<ComponentType T>
		T& GetComponent()
		{
			return reinterpret_cast<T&>(m_Components[typeid(T)]);
		}

		template<ComponentType T>
		void RemoveComponent()
		{
			m_Components.erase()
		}*/

		template<ComponentType T>
		void AddComponent(T* component)
		{
			const std::type_info& typeIdentifier = typeid(T);
			m_Components.insert(std::make_pair(typeIdentifier, component));
		}

	private:
		std::unordered_map<const std::type_info&, Component*> m_Components{};

		Transform m_transform{};
		// todo: mmm, every gameobject has a texture? Is that correct?
		std::shared_ptr<Texture2D> m_texture{};
	};
}
