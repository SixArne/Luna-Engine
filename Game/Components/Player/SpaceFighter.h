#ifndef SPACE_FIGHTER_H
#define SPACE_FIGHTER_H

#include <Core/ECS/Component.h>
#include <memory>
#include <Texture2D.h>
#include <Core/Event/Subject.h>

namespace Galaga
{
	using Engine::Component;
	using Engine::GameObject;
	using Engine::Texture2D;
	using Engine::Subject;

	class SpaceFighter final: public Component, public Subject<int>
	{
	public:
		SpaceFighter(GameObject* object);
		virtual ~SpaceFighter() = default;

		SpaceFighter(const SpaceFighter& other) = delete;
		SpaceFighter(SpaceFighter&& other) = delete;
		SpaceFighter& operator=(const SpaceFighter& other) = delete;
		SpaceFighter& operator=(SpaceFighter&& other) = delete;

		virtual void Init() override;
		virtual void Update() override;
		virtual void Render() override;

        void Shoot();

	private:
		float m_ShootCooldown{0.01f};
		float m_ShootCooldownTimer{};

		int m_Lives{3};

		std::shared_ptr<Texture2D> m_ProjectileTexture{};;
	};
}

#endif
