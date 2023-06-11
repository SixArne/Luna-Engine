#ifndef HIGH_SCORE_INDICATOR_H
#define HIGH_SCORE_INDICATOR_H

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

	class HighscoreIndicator final: public Component, public IObserver<int>
	{
	public:
		HighscoreIndicator(GameObject* gameobject, int beginScore);

		void Init() override;
		void Update() override;
		void Render() override;

		void OnNotify(int data) override;

		void IncreaseScore(int value);

	private:
		TextComponent* m_TextComponent{};
        int m_Score{};
	};
}

#endif // !MOVER_H



