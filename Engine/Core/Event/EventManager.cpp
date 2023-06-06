#include "pch.h"
#include "EventManager.h"

void Engine::EventManager::AttachEvent(const std::string& name, std::function<void(Event*)> pEvent)
{
	m_Events.insert_or_assign(std::string(name.c_str()), pEvent);
}

void Engine::EventManager::DetachEvent(const std::string& name)
{
	m_Events.erase(name.c_str());
}

void Engine::EventManager::Notify(const std::string& name, Event* pEvent)
{
	if (m_Events.contains(name.c_str()))
	{
		m_Events[name.c_str()](pEvent);
	}
}

void Engine::EventManager::ClearEvents()
{
	m_Events.clear();
}
