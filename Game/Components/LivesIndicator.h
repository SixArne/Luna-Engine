#ifndef LIVES_INDICATOR_H
#define LIVES_INDICATOR_H

#include <Core/ECS/TextComponent.h>
#include <Core/Event/EventManager.h>
#include <Core/Event/IObserver.h>

namespace Engine { class TextureRenderer; }

namespace Galaga
{
	using Engine::Component;
	using Engine::GameObject;
	using Engine::TextureRenderer;
	using Engine::IObserver;

	class LivesIndicator final: public Component, public IObserver<int>
	{
	public:
		LivesIndicator(GameObject* gameobject, std::vector<TextureRenderer*>& liveRenderers);

		void Init() override;
		void Update() override;
		void Render() override;
        void OnImGui() override;

		void OnNotify(int data) override;

	private:
        void UpdateLives();

        std::vector<TextureRenderer*> m_LivesIcons{};
        int m_Lives{};
        int m_CurrentLives{};
	};
}

#endif // !MOVER_H



