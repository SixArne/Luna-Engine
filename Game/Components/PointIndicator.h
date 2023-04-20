#ifndef POINT_INDICATER_H
#define POINT_INDICATOR_H

#include <Core/ECS/TextComponent.h>
#include <Core/Event/EventManager.h>
#include <Core/Event/IObserver.h>

namespace Engine { class TextComponent; }

namespace Galaga
{
	using Engine::Component;
	using Engine::GameObject;
	using Engine::TextComponent;
	using Engine::IObserver;

	class PointIndicator final : public Component, public IObserver<int>
	{
	public:
		PointIndicator(GameObject* gameobject, int beginPoints);

		void Init() override;
		void Update() override;
		void Render() override;

		void OnNotify(int data) override;

	private:
		TextComponent* m_TextComponent{};
	};
}

#endif // !MOVER_H



