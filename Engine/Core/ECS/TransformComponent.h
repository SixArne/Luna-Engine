#ifndef TRANSFORM_COMPONENT_H
#define TRANSFORM_COMPONENT_H

#include <glm/vec2.hpp>
#include "Component.h"

namespace Engine
{
	class TransformComponent final: public Component
	{
	public:
		TransformComponent(GameObject* object);
		virtual ~TransformComponent() = default;

		TransformComponent(const TransformComponent& other) = delete;
		TransformComponent(TransformComponent&& other) = delete;
		TransformComponent& operator=(const TransformComponent& other) = delete;
		TransformComponent& operator=(TransformComponent&& other) = delete;

		virtual void ComponentUpdate() override;
		virtual void ComponentRender() override;

		// Position
		const glm::vec2 GetPosition();
		void SetPosition(glm::vec2 position);
		void AddPosition(glm::vec2 offset);

		// Rotation
		const float GetRotation();
		void SetRotation(float rotation);
		void AddRotation(float offset);

	private:
		glm::vec2 m_Position{};
		float m_Rotation{};
	};
}

#endif
