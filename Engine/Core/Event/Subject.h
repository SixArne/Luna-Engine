#ifndef SUBJECT_H
#define SUBJECT_H

#include "pch.h"

namespace Engine
{
	class IObserver;

	class Subject
	{
	public:
		virtual ~Subject() = default;
		virtual void AddObserver(IObserver* observer);
		virtual void RemoveObserver(IObserver* observer);
		virtual void Notify(const std::string& event);
	private:
		std::vector<IObserver*> m_Observers;
	};
}

#endif
