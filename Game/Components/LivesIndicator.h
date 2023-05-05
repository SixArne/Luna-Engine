#ifndef LIVES_INDICATOR_H
#define LIVES_INDICATOR_H

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

	class LivesIndicator : public Component, public IObserver<int>
	{
	public:
		LivesIndicator(GameObject* gameobject, int beginLives);

		void Init() override;
		void Update() override;
		void Render() override;

		void OnNotify(int data) override;

	private:
		TextComponent* m_TextComponent{};
	};
}

#endif // !MOVER_H



