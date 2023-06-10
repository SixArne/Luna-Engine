#ifndef ENGINE_SCENEMANAGER_H
#define ENGINE_SCENEMANAGER_H

#include "Singleton.h"

#include <functional>
#include <vector>
#include <string>
#include <memory>

namespace Engine
{
	class Scene;
	class SceneManager final : public Singleton<SceneManager>
	{
	public:
		Scene* CreateScene(const std::string& name);
		Scene* GetScene(const std::string& name);
		Scene* GetSceneByIndex(unsigned int index);
		Scene* GetActiveScene();
		Scene* GetNextScene();
		Scene* GetPreviousScene();

		void Init();
		void Update();
		void FixedUpdate(float fdt);
		void LateUpdate();
		void Render();
		void OnImGui();

		void OnSceneSwitch(std::function<void(Scene* newScene, Scene* oldScene)> callback);

	private:
		void SwitchScene();

		friend class Singleton<SceneManager>;
		SceneManager() = default;

		Scene* m_ActiveScene = nullptr;
		int m_ActiveSceneIndex = -1;

		std::function<void(Scene* newScene, Scene* oldScene)> m_OnSceneSwitch{[](Scene*, Scene*){return;}};
		std::vector<std::shared_ptr<Scene>> m_scenes;
	};
}

#endif // !ENGINE_SCENEMANAGER_H