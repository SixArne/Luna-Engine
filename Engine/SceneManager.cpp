#include "pch.h"

#include "SceneManager.h"
#include "Scene.h"

void Engine::SceneManager::Init()
{
	m_ActiveScene->Init();
	m_ActiveScene->OnLoad();
	m_OnSceneSwitch(nullptr, nullptr);
}

void Engine::SceneManager::Update()
{
	m_ActiveScene->Update();
	// for(auto& scene : m_scenes)
	// {
	// 	scene->Update();
	// }
}

void Engine::SceneManager::FixedUpdate(float fdt)
{
	m_ActiveScene->FixedUpdate(fdt);
	// for (auto& scene : m_scenes)
	// {
	// 	scene->FixedUpdate(fdt);
	// }
}

void Engine::SceneManager::LateUpdate()
{
	m_ActiveScene->LateUpdate();
	// for (const auto& scene : m_scenes)
	// {
	// 	scene->LateUpdate();
	// }
}

void Engine::SceneManager::Render()
{
	m_ActiveScene->Render();
	// for (const auto& scene : m_scenes)
	// {
	// 	scene->Render();
	// }
}

void Engine::SceneManager::OnImGui()
{
	m_ActiveScene->OnImGui();
	// for (const auto& scene : m_scenes)
	// {
	// 	scene->OnImGui();
	// }
}

Engine::Scene* Engine::SceneManager::CreateScene(const std::string& name)
{
	const auto& scene = std::shared_ptr<Scene>(new Scene(name));
	m_scenes.push_back(scene);

	if (m_ActiveScene == nullptr)
	{
		m_ActiveScene = scene.get();
		m_ActiveSceneIndex = 0;
	}

	return scene.get();
}

Engine::Scene* Engine::SceneManager::GetScene(const std::string& name)
{
	for (const auto& scene : m_scenes)
	{
		if (scene->GetName() == name)
		{
			return scene.get();
		}
	}

	return nullptr;
}

Engine::Scene* Engine::SceneManager::GetActiveScene()
{
	return m_ActiveScene;
}

Engine::Scene* Engine::SceneManager::GetSceneByIndex(unsigned int index)
{
	if (index < m_scenes.size())
	{
		return m_scenes[index].get();
	}

	return nullptr;
}

Engine::Scene* Engine::SceneManager::GetNextScene()
{
	m_ActiveSceneIndex++;

	if (m_ActiveSceneIndex >= m_scenes.size())
	{
		m_ActiveSceneIndex = 0;
	}

	SwitchScene();

	return m_scenes[m_ActiveSceneIndex].get();
}

Engine::Scene* Engine::SceneManager::GetPreviousScene()
{
	m_ActiveSceneIndex--;

	if (m_ActiveSceneIndex < 0)
	{
		m_ActiveSceneIndex = static_cast<int>(m_scenes.size()) - 1;
	}

	SwitchScene();

	return m_scenes[m_ActiveSceneIndex].get();
}

void Engine::SceneManager::OnSceneSwitch(std::function<void(Scene* scene, Scene* oldScene)> callback)
{
	m_OnSceneSwitch = callback;
}

void Engine::SceneManager::SwitchScene()
{
	m_ActiveScene->OnUnload();

	// Copy persistant smart pointers
	const auto persistantObjects = m_ActiveScene->GetPersistantObjects();
	// Clear Persistant smart pointers in old level

	auto oldScene = m_ActiveScene;
	m_ActiveScene = m_scenes[m_ActiveSceneIndex].get();

	// Add them to new level
	for (auto& object: persistantObjects)
	{
		m_ActiveScene->Add(object, true);
	}

	if (!m_ActiveScene->IsInitialized())
	{
		m_ActiveScene->Init();
	}

	m_OnSceneSwitch(m_ActiveScene, oldScene);
	m_ActiveScene->OnLoad();
}