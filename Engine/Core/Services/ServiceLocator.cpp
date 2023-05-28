#include "pch.h"
#include "ServiceLocator.h"
#include "Sound/ISoundService.h"

Engine::ISoundService* Engine::ServiceLocator::m_soundService{&m_defaultSoundService};
Engine::IPhysicsService* Engine::ServiceLocator::m_physicsService{&m_defaultPhysicsService};

void Engine::ServiceLocator::RegisterSoundService(ISoundService* soundService)
{
    m_soundService = (soundService == nullptr)
        ? &m_defaultSoundService
        : soundService;
}

void Engine::ServiceLocator::RegisterPhysicsService(IPhysicsService* physicsService)
{
    m_physicsService = (physicsService == nullptr)
        ? &m_defaultPhysicsService
        : physicsService;
}

Engine::ISoundService* Engine::ServiceLocator::GetSoundService()
{
    return m_soundService;
}

Engine::IPhysicsService* Engine::ServiceLocator::GetPhysicsService()
{
    return m_physicsService;
}

void Engine::ServiceLocator::DestroyServices()
{
    if (m_soundService != &m_defaultSoundService)
    {
        delete m_soundService;
        m_soundService = &m_defaultSoundService;
    }

    if (m_physicsService != &m_defaultPhysicsService)
    {
        delete m_physicsService;
        m_physicsService = &m_defaultPhysicsService;
    }
}
