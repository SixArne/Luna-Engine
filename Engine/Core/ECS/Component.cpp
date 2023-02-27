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
