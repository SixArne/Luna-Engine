#include "pch.h"
#include "Component.h"
#include "GameObject.h"

void Engine::Component::SetOwner(GameObject* owner)
{
	m_Owner = owner;
}

Engine::GameObject* Engine::Component::GetOwner() const
{
	return m_Owner;
}
