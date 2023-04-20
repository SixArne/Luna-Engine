#include "pch.h"
#include "PointComponent.h"

Galaga::PointComponent::PointComponent(GameObject* gameobject)
	:Component{ gameobject }
{
}

Galaga::PointComponent::~PointComponent()
{
}

void Galaga::PointComponent::Init()
{
}

void Galaga::PointComponent::Update()
{
}

void Galaga::PointComponent::Render()
{
}

void Galaga::PointComponent::SetPoints(int points)
{
	m_Points = points;
}

int Galaga::PointComponent::GetPoints() const
{
	return m_Points;
}

void Galaga::PointComponent::AddPoints(int points)
{
	m_Points += points;

	NotifyObservers(m_Points);
}
