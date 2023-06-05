#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include <filesystem>

#include <Engine.h>
#include <SceneManager.h>
#include <Scene.h>
#include <GameObject.h>
#include <ResourceManager.h>
#include <SceneManager.h>
#include <Core/Log.h>
#include <Core/Input/InputManager.h>
#include <memory>

#include <Core/Services/ServiceLocator.h>
#include <Core/Services/Sound/LoggingSoundSystem.h>
#include <Core/Services/Sound/SDLSoundSystem.h>
#include <Core/Services/Physics/PhysicsService.h>

#include "LevelLoader/LevelLoader.h"
#include "LevelInstancer/LevelInstancer.h"

#include "ResourceManager.h"

void load()
{
	constexpr auto windowWidth = 640;
	constexpr auto windowHeight = 480;

	Galaga::LevelLoader levelLoader{};
	Galaga::GameSettings gameSettings{};

	std::vector<Galaga::Level> levels(3);

	levelLoader.LoadGameSettings("Data/Resources/Saved/game_settings.json", gameSettings);

	for (size_t i{}; i < levels.size(); ++i)
	{
		std::string levelName = std::format("Data/Resources/Levels/lvl_{}.json", i);
		levelLoader.LoadLevel(levelName, levels[i]);
	}

	// Needs to be singleton for liveTime
	Galaga::LevelInstancer::GetInstance().Load(levels, gameSettings, std::make_tuple(windowWidth, windowHeight) );


	Engine::SceneManager::GetInstance().OnSceneSwitch([](Engine::Scene* newScene, Engine::Scene* oldScene){
		if (newScene == nullptr || oldScene == nullptr) return;

		if (newScene->GetName() == "menu")
		{
			// Clean up all physics objects
			Engine::ServiceLocator::GetPhysicsService()->CleanAll();

			oldScene->Reset();

			// Disable game input schema
			Engine::InputManager::GetInstance().GetSchema("GAME_SCHEMA")->SetActive(false);

			// Enable menu input schema
			Engine::InputManager::GetInstance().GetSchema("MENU_SCHEMA")->SetActive(true);

			L_DEBUG("Disabling game controls, enabling menu controls");
		}
		else
		{
			// Enable game input schema and disable menu input schema
			Engine::InputManager::GetInstance().GetSchema("GAME_SCHEMA")->SetActive(true);

			// Disable menu input schema
			Engine::InputManager::GetInstance().GetSchema("MENU_SCHEMA")->SetActive(false);

			L_DEBUG("Enabling game controls, disabling menu controls");
		}
	});
}

int main(int, char*[]) {
	using Engine::LoggingSoundSystem;
	using Engine::SDLSoundSystem;
	using Engine::PhysicsService;

	Engine::Log::Init();

	#ifdef DEBUG
	Engine::ServiceLocator::RegisterSoundService(new LoggingSoundSystem(new SDLSoundSystem(4)));
	#else
	Engine::ServiceLocator::RegisterSoundService(new SDLSoundSystem(4));
	#endif

	Engine::ServiceLocator::RegisterPhysicsService(new PhysicsService());

	auto cwd = std::filesystem::current_path();

	L_DEBUG("Current path: {0}", cwd.string());

	Engine::Engine engine("./Data/");
	engine.Run(load);

	Engine::ServiceLocator::DestroyServices();

	return 0;
}