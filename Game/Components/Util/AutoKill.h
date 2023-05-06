#ifndef AUTOKILL_H
#define AUTOKILL_H

#include <Core/ECS/Component.h>

namespace Galaga
{
	using Engine::Component;
	using Engine::GameObject;

	class AutoKill final: public Component
	{
	public:
		AutoKill(GameObject* object, float lifetime);
		virtual ~AutoKill() = default;

		AutoKill(const AutoKill& other) = delete;
		AutoKill(AutoKill&& other) = delete;
		AutoKill& operator=(const AutoKill& other) = delete;
		AutoKill& operator=(AutoKill&& other) = delete;

		virtual void Init() override;
		virtual void Update() override;
		virtual void Render() override;

    private:
        float m_Lifetime{};
        float m_TimeAlive{};
	};
}

#endif
