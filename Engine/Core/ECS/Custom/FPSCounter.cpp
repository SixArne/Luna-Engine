#include "pch.h"
#include "FPSCounter.h"

#include "GameObject.h"
#include "Core/Time.h"
#include "Core/ECS/TextComponent.h"

void Engine::FPSCounter::ComponentInit()
{
	const auto gameObject = GetOwner();

	if (!gameObject->HasComponent<TextComponent>())
	{
		L_ERROR("[{}] FPSCounter requires [TextComponent] to be attached on the same GameObject.", GetOwner()->GetName())
	}
}

void Engine::FPSCounter::ComponentUpdate()
{
	float deltaTime = TIME.GetDeltaTime();
	m_TimeSinceLastFPSPush += deltaTime;

	if (m_TimeSinceLastFPSPush >= 0.5f)
	{
		auto textComponent = GetOwner()->GetComponent<TextComponent>();
		if (textComponent != nullptr)
		{
			float FPS = 1.0f / deltaTime;
			textComponent->SetText(std::format("{} FPS", std::to_string((int)FPS)));
		}

		m_TimeSinceLastFPSPush = 0.f;
	}
}

void Engine::FPSCounter::ComponentRender()
{}
