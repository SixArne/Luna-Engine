#include "pch.h"
#include "PointCommand.h"
#include "GameObject.h"
#include "Core/ECS/Custom/PointComponent.h"

#pragma warning(push)
#pragma warning(disable: 4996)
#pragma warning(disable: 4389)
#include "Core/Steam/CSteamAchievements.h"
#pragma warning(pop)


void Engine::PointCommand::Execute()
{
	auto pointComponent = m_GameObject->GetComponent<PointComponent>();
	if (pointComponent != nullptr)
	{
		pointComponent->AddPoints(1);
	}

	CSteamAchievements::GetInstance().SetAchievement("ACH_WIN_100_GAMES");
}
