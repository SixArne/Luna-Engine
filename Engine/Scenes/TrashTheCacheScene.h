#ifndef TRASH_THE_CACHE_SCENE_H
#define TRASH_THE_CACHE_SCENE_H

#include "Scene.h"

namespace Engine
{
	struct Transform
	{
		float matrix[16]
		{
			1,0,0,0,
			0,1,0,0,
			0,0,1,0,
			0,0,0,1
		};
	};

	class GameObject3D
	{
	public:
		GameObject3D(int id)
			:ID{ id }
		{

		}
		Transform transform;
		int ID;
	};

	class GameObject3DAlt
	{
	public:
		GameObject3DAlt(int id)
			:ID{ id }
		{
		}
		std::unique_ptr<Transform> transform{ std::make_unique<Transform>() };
		int ID;
	};

	class TrashTheCacheScene : public Scene
	{
	public:
		void OnImGui() override;
		explicit TrashTheCacheScene(const std::string& scenename);

	private:
		void BenchMarkExercise1();
		void BenchMarkExercise2();
		void BenchMarkExercise2Alt();

		std::string m_name{"GRAPH_DEFAULT"};

		std::vector<double> m_Iterations{};
		 
		std::vector<double> m_Ex1Results{};
		

		std::vector<double> m_Ex2Results{};
		std::vector<double> m_Ex2AltResults{};
	};

}

#endif

