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

		float GetTotalTime() const;
		float GetDeltaTime() const;

	private:
		float m_DeltaTime{};
		double m_TotalTime{};
	};
}

#endif // !TIME_H
