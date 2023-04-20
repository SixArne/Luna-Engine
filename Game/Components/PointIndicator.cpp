#include "pch.h"
#include "PointIndicator.h"
#include "GameObject.h"

#include "Core/Event/EventManager.h"
#include "Core/ECS/TextComponent.h"

Engine::PointIndicator::PointIndicator(GameObject* gameobject, int beginPoints)
	: TextComponent(gameobject)
{
	auto currentPointsText = std::format("{} points", beginPoints);
	SetText(currentPointsText);
}

void Engine::PointIndicator::Init()
{
	TextComponent::Init();
}

void Engine::PointIndicator::Update()
{
	TextComponent::Update();
}

void Engine::PointIndicator::Render()
{
	TextComponent::Render();
}

void Engine::PointIndicator::OnNotify(int data)
{
	auto currentPointsText = std::format("{} points", data);
	SetText(currentPointsText);
}