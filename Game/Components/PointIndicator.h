#ifndef POINT_INDICATER_H
#define POINT_INDICATOR_H

#include <Core/ECS/TextComponent.h>
#include <Core/Event/EventManager.h>
#include <Core/Event/IObserver.h>

namespace Engine
{

	class PointIndicator final :public TextComponent, public IObserver<int>
	{
	public:
		PointIndicator(GameObject* gameobject, int beginPoints);

		void Init() override;
		void Update() override;
		void Render() override;

		void OnNotify(int data) override;

		IObserver<int>* GetObserver() override {
			return this;
		}
	};
}

#endif // !MOVER_H



