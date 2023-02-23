#include "pch.h"
#include "Time.h"

void Engine::Time::Update(float deltaTime)
{
    m_DeltaTime = deltaTime;
    m_TotalTime += (double)deltaTime;
}

float Engine::Time::GetTotalTime() const
{
    return 0.0f;
}

float Engine::Time::GetDeltaTime() const
{
    return 0.0f;
}
