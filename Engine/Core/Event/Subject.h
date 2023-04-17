#ifndef SUBJECT_H
#define SUBJECT_H

#include "pch.h"
#include "Core/Event/IObserver.h"


namespace Engine
{
	template<typename T>
	class Subject
	{
	public:
		// Function to register an observer
		void RegisterObserver(IObserver<T>* observer)
		{
			m_Observers.push_back(observer);
		}

		// Function to remove an observer
		void RemoveObserver(IObserver<T>* observer)
		{
			m_Observers.erase(std::remove(m_Observers.begin(), m_Observers.end(), observer), m_Observers.end());
		}

		// Function to notify all observers of a change
		void NotifyObservers(T data)
		{
			for (auto observer : m_Observers)
			{
				observer->OnNotify(data);
			}
		}

	private:
		std::vector<IObserver<T>*> m_Observers;
	};
}

#endif
