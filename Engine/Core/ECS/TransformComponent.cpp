#include "pch.h"
#include "TransformComponent.h"
#include "GameObject.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>

#pragma warning(push)
#pragma warning(disable: 4201)
#include <glm/gtx/quaternion.hpp>
#pragma warning(pop)

#include "ImGui.h"

Engine::TransformComponent::TransformComponent(GameObject* object)
	: Component{ object }
{
}

Engine::TransformComponent::TransformComponent(GameObject* object, glm::vec2 position)
	: Component{ object }, m_LocalPosition{ position }
{
}

Engine::TransformComponent::TransformComponent(GameObject* object, glm::vec2 position, float rotation)
	: Component{object}, m_LocalPosition{ position }, m_LocalRotation{ rotation }
{
}

void Engine::TransformComponent::Update()
{
}

void Engine::TransformComponent::Render()
{
}

void Engine::TransformComponent::Init()
{
}

void Engine::TransformComponent::AddLocalPosition(const glm::vec2& offset)
{
	m_LocalPosition += offset;
	SetPositionDirty();
}

void Engine::TransformComponent::AddLocalRotation(const float offset)
{
	m_LocalRotation += offset;
	SetPositionDirty();
}

void Engine::TransformComponent::SetLocalPosition(const glm::vec2& pos)
{
	m_LocalPosition = pos;
	SetPositionDirty();
}

void Engine::TransformComponent::SetLocalRotation(const float angle)
{
	m_LocalRotation = angle;
	SetPositionDirty();
}

void Engine::TransformComponent::SetLocalScale(const glm::vec2& scale)
{
	m_LocalScale = scale;
	SetPositionDirty();
}

const glm::vec2& Engine::TransformComponent::GetLocalPosition()
{
	return m_LocalPosition;
}

const glm::vec2& Engine::TransformComponent::GetLocalScale()
{
	return m_LocalScale;
}

const float Engine::TransformComponent::GetLocalRotation()
{
	return m_LocalRotation;
}

void Engine::TransformComponent::SetPositionDirty()
{
	m_IsDirty = true;

	auto children = GetOwner()->GetChildren();
	for (auto& child : children)
	{
		child->GetTransform()->SetPositionDirty();
	}
}

const glm::mat4 Engine::TransformComponent::UpdateWorldTransform()
{
	auto owner = GetOwner()->GetParent();

	if (m_IsDirty)
	{
		glm::mat4 localTransform = glm::mat4(1.0);
		localTransform = glm::translate(localTransform, glm::vec3(m_LocalPosition, 0.0f));
		localTransform = glm::rotate(localTransform, glm::radians(m_LocalRotation), glm::vec3(0.0f, 0.0f, 1.0f));
		localTransform = glm::scale(localTransform, glm::vec3(m_LocalScale, 1.0f));

		if (owner == nullptr)
		{
			m_Transform = localTransform;
		}
		else
		{
			auto parentWorldTransform = owner->GetTransform()->UpdateWorldTransform();
			m_Transform = parentWorldTransform * localTransform;
		}

		// Update world position, scale and rotation for all
		m_WorldPosition = GetPositionFromMatrix(m_Transform);
		m_WorldRotation = GetRotationFromMatrix(m_Transform);
		m_WorldScale = GetScaleFromMatrix(m_Transform);
	}

	m_IsDirty = false;
	return m_Transform;
}

const glm::vec2& Engine::TransformComponent::GetWorldPosition()
{
	if (m_IsDirty)
	{
		UpdateWorldTransform();
	}

	return m_WorldPosition;
}

const float Engine::TransformComponent::GetWorldRotation()
{
	if (m_IsDirty)
	{
		UpdateWorldTransform();
	}

	return m_WorldRotation;
}

const glm::vec2& Engine::TransformComponent::GetWorldScale()
{
	if (m_IsDirty)
	{
		UpdateWorldTransform();
	}

	return m_WorldScale;
}

const glm::mat4& Engine::TransformComponent::GetWorldTransform()
{
	return m_Transform;
}

// Helpers
glm::vec2 Engine::TransformComponent::GetPositionFromMatrix(const glm::mat4& matrix)
{
	return glm::vec2(matrix[3][0], matrix[3][1]);
}

float Engine::TransformComponent::GetRotationFromMatrix(const glm::mat4& matrix)
{
	glm::vec3 euler = glm::eulerAngles(glm::quat_cast(matrix));
	return glm::degrees(euler.z);
}

glm::vec2 Engine::TransformComponent::GetScaleFromMatrix(const glm::mat4& matrix)
{
	return glm::vec2(glm::length(matrix[0]), glm::length(matrix[1]));
}

void Engine::TransformComponent::OnImGui()
{
	if (ImGui::CollapsingHeader("Transform"))
	{
		ImGui::BeginChild("Transform");

		ImGui::Text("Transform");

		auto location = m_LocalPosition;
		auto rotation = m_LocalRotation;
		auto scale = m_LocalScale;

		ImGui::DragFloat2("Position", &location.x, 0.1f);
		ImGui::DragFloat("Rotation", &rotation, 0.1f);
		ImGui::DragFloat2("Scale", &scale.x, 0.1f);

		if (glm::length(location - m_LocalPosition) >= FLT_EPSILON)
		{
			SetLocalPosition(location);
		}

		if (glm::abs(rotation - m_LocalRotation) >= FLT_EPSILON)
		{
			SetLocalRotation(rotation);
		}

		if (glm::length(scale - m_LocalScale) >= FLT_EPSILON)
		{
			SetLocalScale(scale);
		}


		ImGui::EndChild();
	}
	
}
