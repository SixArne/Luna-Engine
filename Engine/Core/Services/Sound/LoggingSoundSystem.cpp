#include "pch.h"
#include "LoggingSoundSystem.h"
#include <Core/Log.h>

Engine::LoggingSoundSystem::LoggingSoundSystem(ISoundService* soundService)
    : m_soundService{soundService}
{
    L_INFO("Logging sound system created");
}

Engine::LoggingSoundSystem::~LoggingSoundSystem()
{
    L_INFO("Logging sound system destroyed");
    delete m_soundService;
}

void Engine::LoggingSoundSystem::Play(const std::string& soundName, const float volume)
{
    L_INFO("Playing sound: {0} at volume: {1}", soundName, volume);
    m_soundService->Play(soundName, volume);
}

void Engine::LoggingSoundSystem::IncreaseVolume()
{
    L_INFO("Increasing volume");
    m_soundService->IncreaseVolume();
}

void Engine::LoggingSoundSystem::DecreaseVolume()
{
    L_INFO("Decreasing volume");
    m_soundService->DecreaseVolume();
}

void Engine::LoggingSoundSystem::Mute()
{
    L_INFO("Muting");
    m_soundService->Mute();
}