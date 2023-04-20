#include "pch.h"
#include "PointCommand.h"
#include <GameObject.h>
#include "Components/PointComponent.h"

void Engine::PointCommand::Execute(InputData)
{
	auto pointComponent = m_GameObject->GetComponent<PointComponent>();
	if (pointComponent != nullptr)
	{
		pointComponent->AddPoints(1);
	}

}
