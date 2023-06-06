#ifndef EVENT_MANAGER_H
#define EVENT_MANAGER_H

#include "pch.h"
#include "Singleton.h"

namespace Engine
{
	struct Event
	{
		const char* name;
	};

	class EventManager final : public Singleton<EventManager>
	{
	public:
		void AttachEvent(const std::string& name, std::function<void(Event*)> pEventCallback);
		void DetachEvent(const std::string& name);
		void Notify(const std::string& name, Event* pEvent);

		void ClearEvents();

	private:
		friend class Singleton<EventManager>;
		EventManager() = default;
		std::unordered_map<std::string, std::function<void(Event*)>> m_Events;
	};
}

#endif
