#ifndef TIME_H
#define TIME_H

#define TIME Time::GetInstance()

#include "Singleton.h"

namespace Engine
{
	class Time final: public Singleton<Time>
	{
	public:
		void Update(float deltaTime);

		float GetDeltaTime() const;
		double GetElapsedTime() const;

	private:
		float m_DeltaTime{};
		double m_ElapsedTime{};
	};
}

#endif
