#include "pch.h"
#include "Component.h"
#include "GameObject.h"

//void Engine::Component::SetOwner(GameObject* owner)
//{
//	m_pOwner = owner;
//}

Engine::GameObject* Engine::Component::GetOwner() const
{
	return m_pOwner;
}

void Engine::Component::SetCanBeRemoved(bool value)
{
	m_CanBeRemoved = value;
}

bool Engine::Component::GetCanBeRemoved()
{
	return m_CanBeRemoved;
}
