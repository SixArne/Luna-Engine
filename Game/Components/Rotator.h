#ifndef ROTATOR_H
#define ROTATOR_H

#include <Core/ECS/Component.h>

namespace Engine
{
	class TransformComponent;

	class Rotator : public Component
	{
	public:
		Rotator(GameObject* gameobject);

		void Init() override;
		void Update() override;
		void Render() override;
	private:
		TransformComponent* m_TransformComponent{};
	};
}

#endif // !Rotator
