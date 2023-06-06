#ifndef ENEMY_BUG_H
#define ENEMY_BUG_H

#include <Core/ECS/Component.h>
#include <memory>
#include <Texture2D.h>
#include "Scene.h"

namespace Galaga
{
	using Engine::Component;
	using Engine::GameObject;
	using Engine::Texture2D;

	class EnemyBug final: public Component
	{
	public:
		EnemyBug(GameObject* object, glm::vec2 targetPosition);
		virtual ~EnemyBug() = default;

		EnemyBug(const EnemyBug& other) = delete;
		EnemyBug(EnemyBug&& other) = delete;
		EnemyBug& operator=(const EnemyBug& other) = delete;
		EnemyBug& operator=(EnemyBug&& other) = delete;

		virtual void Init() override;
		virtual void Update() override;
		virtual void Render() override;

		glm::vec2 GetTargetPosition() const { return m_TargetPosition; }

	private:
		float m_ShootCooldown{0.3f};
		float m_ShootCooldownTimer{};

		glm::vec2 m_TargetPosition{};
		std::shared_ptr<Texture2D> m_ProjectileTexture{};
	};
}

#endif
