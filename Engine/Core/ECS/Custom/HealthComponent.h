#ifndef HEALTH_COMPONENT_H
#define HEALTH_COMPONENT_H

#include "Core/ECS/Component.h"

namespace Engine
{
    class HealthComponent : public Component
    {
	public:
		HealthComponent(GameObject* gameobject);
		~HealthComponent();
		void Init() override;
		void Update() override;
		void Render() override;

		void SetHealth(int health);
		int GetHealth() const;
		void TakeDamage(int damage);

	private:
		int m_Health{ 100 };
	};
}


#endif