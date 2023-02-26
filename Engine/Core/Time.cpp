#include "pch.h"
#include "Time.h"

void Engine::Time::Update(float deltaTime)
{
	m_DeltaTime = deltaTime;
}

float Engine::Time::GetDeltaTime() const
{
	return m_DeltaTime;
}

double Engine::Time::GetElapsedTime() const
{
	return m_ElapsedTime;
}

