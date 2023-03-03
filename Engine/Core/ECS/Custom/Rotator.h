#ifndef ROTATOR_H
#define ROTATOR_H

#include "Core/ECS/Component.h"

namespace Engine
{
	class Rotator : public Component
	{
	public:
		Rotator(GameObject* gameobject);

		void ComponentUpdate() override;
		void ComponentRender() override;
	};
}

#endif // !Rotator
