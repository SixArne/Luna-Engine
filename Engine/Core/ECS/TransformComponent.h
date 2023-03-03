#ifndef TRANSFORM_COMPONENT_H
#define TRANSFORM_COMPONENT_H

#include <glm/vec2.hpp>
#include <glm/mat3x3.hpp>
#include "Component.h"

namespace Engine
{
	class TransformComponent final: public Component
	{
	public:
		TransformComponent(GameObject* object);
		TransformComponent(GameObject* object, glm::vec2 position);
		TransformComponent(GameObject* object, glm::vec2 position, float rotation);
		
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

		void SetLocalPosition(const glm::vec2& pos);
		void SetLocalRotation(const float angle);

		const glm::vec2& GetLocalPosition();
		float GetLocalRotation();

		const glm::vec2& GetWorldPosition();
		void SetPositionDirty();
		void UpdateWorldPosition();

	private:
		glm::vec2 m_LocalPosition{};
		float m_LocalRotation{};

		glm::vec2 m_WorldPosition{};

		bool m_IsPositionDirty{};
	};
}

#endif
