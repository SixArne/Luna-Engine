#pragma once
#include "SceneManager.h"

namespace Engine
{
	class GameObject;
	class Scene final
	{
		friend Scene& SceneManager::CreateScene(const std::string& name);
	public:
		explicit Scene(const std::string& name);
		void Add(std::shared_ptr<GameObject> object);
		void Remove(std::shared_ptr<GameObject> object);
		void RemoveAll();

		void Init();
		void Update();
		void FixedUpdate(float fdt);
		void LateUpdate();
		void Render() const;
		virtual void OnImGui();

		const std::string& GetName() const { return m_name; }

		void Instantiate(std::shared_ptr<GameObject> object);

		~Scene();
		Scene(const Scene& other) = delete;
		Scene(Scene&& other) = delete;
		Scene& operator=(const Scene& other) = delete;
		Scene& operator=(Scene&& other) = delete;

	protected:
		std::string m_name;
		std::vector<std::shared_ptr<GameObject>> m_objects{};
		std::vector<std::shared_ptr<GameObject>> m_objectsToDestroy{};

		static unsigned int m_idCounter;
	};

}
