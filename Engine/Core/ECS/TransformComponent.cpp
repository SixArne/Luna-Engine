#include "pch.h"
#include "TransformComponent.h"

Engine::TransformComponent::TransformComponent(GameObject* object)
	: Component(object)
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
	return m_Position;
}

void Engine::TransformComponent::SetPosition(glm::vec2 position)
{
	m_Position = position;
}

void Engine::TransformComponent::AddPosition(glm::vec2 offset)
{
	m_Position += offset;
}

const float Engine::TransformComponent::GetRotation()
{
	return m_Rotation;
}

void Engine::TransformComponent::SetRotation(float rotation)
{
	m_Rotation = rotation;
}

void Engine::TransformComponent::AddRotation(float offset)
{
	m_Rotation += offset;
}
