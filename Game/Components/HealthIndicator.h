#ifndef HEALTH_INDICATER_H
#define HEALTH_INDICATOR_H

#include <Core/ECS/TextComponent.h>
#include <Core/Event/EventManager.h>
#include <Core/Event/IObserver.h>

namespace Engine
{

	class HealthIndicator :public TextComponent, public IObserver<int>
	{
	public:
		HealthIndicator(GameObject* gameobject, int beginHealth);

		void Init() override;
		void Update() override;
		void Render() override;

		void OnNotify(int data) override;
	};
}

#endif // !MOVER_H



