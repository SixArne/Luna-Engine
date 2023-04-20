#include "pch.h"
#include "DieCommand.h"
#include "Components/HealthComponent.h"
#include <GameObject.h>
#include <Core/Input/Command.h>


void Engine::DieCommand::Execute(InputData)
{
	auto healthComponent = m_GameObject->GetComponent<HealthComponent>();
	healthComponent->TakeDamage(1);
}