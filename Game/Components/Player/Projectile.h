#ifndef PROJECTILE_FIGHTER_H
#define PROJECTILE_FIGHTER_H

#include <Core/ECS/Component.h>
#include <glm/vec2.hpp>

namespace Galaga
{
	using Engine::Component;
	using Engine::GameObject;

	class Projectile final: public Component
	{
	public:
		Projectile(GameObject* object, glm::vec2 direction, glm::vec2 speed);
		virtual ~Projectile() = default;

		Projectile(const Projectile& other) = delete;
		Projectile(Projectile&& other) = delete;
		Projectile& operator=(const Projectile& other) = delete;
		Projectile& operator=(Projectile&& other) = delete;

		virtual void Init() override;
		virtual void Update() override;
		virtual void Render() override;

    private:
        glm::vec2 m_Direction;
        glm::vec2 m_Speed;
	};
}

#endif
