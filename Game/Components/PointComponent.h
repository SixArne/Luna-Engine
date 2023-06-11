#ifndef POINT_COMPONENT_H
#define POINT_COMPONENT_H

#include <Core/ECS/Component.h>
#include <Core/Event/Subject.h>

namespace Galaga
{
	using Engine::Component;
	using Engine::GameObject;
	using Engine::Subject;

	class PointComponent final: public Component, public Subject<int>
	{
	public:
		PointComponent(GameObject* gameobject);
		~PointComponent();
		void Init() override;
		void Update() override;
		void Render() override;

		void SetPoints(int points);
		int GetPoints() const;
		void AddPoints(int points);

	private:
		int m_Points{ 3 };
	};
}


#endif