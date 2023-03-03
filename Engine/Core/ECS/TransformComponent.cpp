#include "pch.h"
#include "TransformComponent.h"
#include "GameObject.h"

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

void Engine::TransformComponent::ComponentUpdate()
{
}

void Engine::TransformComponent::ComponentRender()
{
}

const glm::vec2 Engine::TransformComponent::GetPosition()
{
	return m_LocalPosition;
}

void Engine::TransformComponent::SetPosition(glm::vec2 position)
{
	m_LocalPosition = position;
	SetPositionDirty();
}

void Engine::TransformComponent::AddPosition(glm::vec2 offset)
{
	m_LocalPosition += offset;
	SetPositionDirty();
}

const float Engine::TransformComponent::GetRotation()
{
	return m_LocalRotation;
}

void Engine::TransformComponent::SetRotation(float rotation)
{
	m_LocalRotation = rotation;
	SetPositionDirty();
}

void Engine::TransformComponent::AddRotation(float offset)
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

const glm::vec2& Engine::TransformComponent::GetLocalPosition()
{
	return m_LocalPosition;
}

float Engine::TransformComponent::GetLocalRotation()
{
	return m_LocalRotation;
}

const glm::vec2& Engine::TransformComponent::GetWorldPosition()
{
	if (m_IsPositionDirty)
	{
		UpdateWorldPosition();
	}

	return m_WorldPosition;
}

void Engine::TransformComponent::SetPositionDirty()
{
	m_IsPositionDirty = true;

	auto children = GetOwner()->GetChildren();
	for (auto child : children)
	{
		child->GetTransform()->SetPositionDirty();
	}
}

void Engine::TransformComponent::UpdateWorldPosition()
{
	auto owner = GetOwner()->GetParent();

	if (m_IsPositionDirty)
	{
		if (owner == nullptr)
		{
			m_WorldPosition = m_LocalPosition;
		}
		else
		{
			auto parentPosition = owner->GetComponent<TransformComponent>()->GetWorldPosition();
			m_WorldPosition = parentPosition + m_LocalPosition;
		}
	}

	m_IsPositionDirty = false;
}
