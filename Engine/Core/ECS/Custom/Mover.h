#ifndef MOVER_H
#define MOVER_H

#include "Core/ECS/Component.h"

namespace Engine
{
	class Mover :public Component
	{
	public:
		Mover(GameObject* gameobject);

		void Init() override;
		void Update() override;
		void Render() override;
	};
}

#endif // !MOVER_H



