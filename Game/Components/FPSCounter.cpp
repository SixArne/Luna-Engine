#include "pch.h"
#include "FPSCounter.h"

#include "GameObject.h"
#include "Core/Time.h"
#include "Core/ECS/TextComponent.h"

Engine::FPSCounter::FPSCounter(GameObject* object)
	: Component{object}
{
}

void Engine::FPSCounter::Init()
{
	const auto gameObject = GetOwner();

	if (!gameObject->HasComponent<TextComponent>())
	{
		L_ERROR("[{}] FPSCounter requires [TextComponent] to be attached on the same GameObject.", GetOwner()->GetName())
	}
	else
	{
		m_TextComponent = gameObject->GetComponent<TextComponent>();
	}
}

void Engine::FPSCounter::Update()
{
	float deltaTime = TIME.GetDeltaTime();
	m_TimeSinceLastFPSPush += deltaTime;

	if (m_TimeSinceLastFPSPush >= 0.5f)
	{
		if (m_TextComponent != nullptr)
		{
			float FPS = 1.0f / deltaTime;
			m_TextComponent->SetText(std::format("{} FPS", std::to_string((int)FPS)));
		}

		m_TimeSinceLastFPSPush = 0.f;
	}
}

void Engine::FPSCounter::Render()
{}
