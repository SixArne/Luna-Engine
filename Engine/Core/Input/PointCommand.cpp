#include "pch.h"
#include "PointCommand.h"
#include "GameObject.h"
#include "Core/ECS/Custom/PointComponent.h"

void Engine::PointCommand::Execute()
{
	auto pointComponent = m_GameObject->GetComponent<PointComponent>();
	if (pointComponent != nullptr)
	{
		pointComponent->AddPoints(1);
	}
}
