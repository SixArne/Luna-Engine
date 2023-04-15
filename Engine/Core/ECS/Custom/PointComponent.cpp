#include "pch.h"
#include "PointComponent.h"

Engine::PointComponent::PointComponent(GameObject* gameobject)
	:Component{ gameobject }
{
}

Engine::PointComponent::~PointComponent()
{
}

void Engine::PointComponent::Init()
{
}

void Engine::PointComponent::Update()
{
}

void Engine::PointComponent::Render()
{
}

void Engine::PointComponent::SetPoints(int points)
{
	m_Points = points;
}

int Engine::PointComponent::GetPoints() const
{
	return m_Points;
}

void Engine::PointComponent::AddPoints(int points)
{
	m_Points += points;

	NotifyObservers(m_Points);
}
