#include "PointCommand.h"
#include <GameObject.h>
#include "Components/PointComponent.h"

using Galaga::PointComponent;

void Galaga::PointCommand::Execute(InputData)
{
	auto pointComponent = m_GameObject->GetComponent<PointComponent>();
	if (pointComponent != nullptr)
	{
		pointComponent->AddPoints(1);
	}
}
