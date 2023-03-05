#include "pch.h"
#include "Rotator.h"
#include "GameObject.h"
#include "Core/Time.h"
#include "Core/ECS/TransformComponent.h"

Engine::Rotator::Rotator(GameObject* gameobject)
	: Component(gameobject)
{
}

void Engine::Rotator::Init()
{
	m_TransformComponent = GetOwner()->GetTransform();
}

void Engine::Rotator::Update()
{
	m_TransformComponent->AddLocalPosition(glm::vec2{10,0} * TIME.GetDeltaTime());
}

void Engine::Rotator::Render()
{
	
}
