#include "pch.h"
#include "Mover.h"
#include "Core/Input/InputManager.h"
#include "Core/Time.h"
#include "GameObject.h"

Engine::Mover::Mover(GameObject* gameobject)
	: Component(gameobject)
{
}

void Engine::Mover::Init()
{
}

void Engine::Mover::Update()
{
	glm::vec2 moveInput = InputManager::GetInstance().GetController()->GetLeftThumbStick();

	L_DEBUG("Move: x {} y {}", moveInput.x, moveInput.y);

	GetOwner()->GetTransform()->AddLocalPosition(moveInput * 50.f * TIME.GetDeltaTime());
}

void Engine::Mover::Render()
{
}
