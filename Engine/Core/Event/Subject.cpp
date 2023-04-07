#include "pch.h"
#include "Subject.h"
#include "Core/Event/IObserver.h"

void Engine::Subject::AddObserver(IObserver* observer)
{
	m_Observers.emplace_back(observer);
}

void Engine::Subject::RemoveObserver(IObserver* observer)
{
	m_Observers.erase(std::remove(begin(m_Observers), end(m_Observers), observer));
}

void Engine::Subject::Notify(const std::string& event)
{
	for (IObserver* observer : m_Observers)
	{
		observer->OnNotify(event);
	}
}
