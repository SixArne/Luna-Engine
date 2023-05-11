#include "pch.h"
#include "ServiceLocator.h"
#include "Sound/ISoundService.h"

Engine::ISoundService* Engine::ServiceLocator::m_soundService{&m_defaultSoundService};

void Engine::ServiceLocator::RegisterSoundService(ISoundService* soundService)
{
    m_soundService = (soundService == nullptr)
        ? &m_defaultSoundService
        : soundService;
}

Engine::ISoundService* Engine::ServiceLocator::GetSoundService()
{
    return m_soundService;
}

void Engine::ServiceLocator::DestroyServices()
{
    if (m_soundService != &m_defaultSoundService)
    {
        delete m_soundService;
        m_soundService = &m_defaultSoundService;
    }
}
