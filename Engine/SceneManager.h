#pragma once

#include "pch.h"
#include "Singleton.h"

namespace Engine
{
	class Scene;
	class SceneManager final : public Singleton<SceneManager>
	{
	public:
		Scene& CreateScene(const std::string& name);
		Scene* GetScene(const std::string& name);

		Scene* GetActiveScene();

		void Init();
		void Update();
		void FixedUpdate(float fdt);
		void LateUpdate();
		void Render();
		void OnImGui();

	private:
		friend class Singleton<SceneManager>;
		SceneManager() = default;

		Scene* m_activeScene = nullptr;
		std::vector<std::shared_ptr<Scene>> m_scenes;
	};
}
