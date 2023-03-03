#include "pch.h"
#include "Rotator.h"
#include "GameObject.h"
#include "Core/Time.h"

Engine::Rotator::Rotator(GameObject* gameobject)
	: Component(gameobject)
{
}

void Engine::Rotator::ComponentUpdate()
{
	auto transform = GetOwner()->GetTransform();

	transform->AddPosition(glm::vec2{10,0} * TIME.GetDeltaTime());
}

void Engine::Rotator::ComponentRender()
{
	
}
