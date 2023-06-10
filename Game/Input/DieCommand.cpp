#include "DieCommand.h"
#include "Components/HealthComponent.h"
#include <GameObject.h>
#include <Core/Input/Command.h>

using Galaga::HealthComponent;


void Galaga::DieCommand::Execute(InputData)
{
	auto healthComponent = m_GameObject->GetComponent<HealthComponent>();
	healthComponent->TakeDamage(1);
}