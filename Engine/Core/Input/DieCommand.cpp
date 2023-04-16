#include "pch.h"
#include "DieCommand.h"
#include "Core/ECS/Custom/HealthComponent.h"
#include "GameObject.h"


void Engine::DieCommand::Execute(InputData)
{
	auto healthComponent = m_GameObject->GetComponent<HealthComponent>();
	healthComponent->TakeDamage(1);
}