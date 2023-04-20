#ifndef FPS_COUNTER_H
#define FPS_COUNTER_H

#include <Core/ECS/Component.h>

namespace Engine { class TextComponent; }

namespace Galaga
{
	using Engine::Component;
	using Engine::GameObject;
	using Engine::TextComponent;

	class FPSCounter final: public Component
	{
	public:
		FPSCounter(GameObject* object);
		virtual ~FPSCounter() = default;

		FPSCounter(const FPSCounter& other) = delete;
		FPSCounter(FPSCounter&& other) = delete;
		FPSCounter& operator=(const FPSCounter& other) = delete;
		FPSCounter& operator=(FPSCounter&& other) = delete;

		virtual void Init() override;
		virtual void Update() override;
		virtual void Render() override;

	private:
		float m_TimeSinceLastFPSPush{};
		TextComponent* m_TextComponent{};
	};
}

#endif
