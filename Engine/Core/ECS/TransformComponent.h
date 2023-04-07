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

		virtual void Init() override;
		virtual void Update() override;
		virtual void Render() override;

		// getters
		const glm::vec2& GetLocalPosition();
		const glm::vec2& GetLocalScale();
		const float GetLocalRotation();

		// adders
		void AddLocalPosition(const glm::vec2& offset);
		void AddLocalRotation(const float offset);
		
		// setters
		void SetLocalPosition(const glm::vec2& pos);
		void SetLocalRotation(const float angle);
		void SetLocalScale(const glm::vec2& scale);

		void SetPositionDirty();

		// getters world
		const glm::vec2& GetWorldPosition();
		const float GetWorldRotation();
		const glm::vec2& GetWorldScale();
		const glm::mat4& GetWorldTransform();

	private:
		const glm::mat4 UpdateWorldTransform();

		glm::vec2 GetPositionFromMatrix(const glm::mat4& matrix);
		float GetRotationFromMatrix(const glm::mat4& matrix);
		glm::vec2 GetScaleFromMatrix(const glm::mat4& matrix);

		void OnImGui() override;

		glm::vec2 m_LocalPosition{};
		float m_LocalRotation{};
		glm::vec2 m_LocalScale{1.0f, 1.0f};

		glm::vec2 m_WorldPosition{};
		float m_WorldRotation{};
		glm::vec2 m_WorldScale{1.0f, 1.0f};

		glm::mat4 m_Transform{ glm::mat4()};
		bool m_IsDirty{true};
	};
}

#endif
