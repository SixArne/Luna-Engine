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
#include <Core/Input/InputManager.h>
#include <Core/Input/XboxController.h>
#include <Core/ECS/TextureRenderer.h>
#include <Core/ECS/TextComponent.h>
#include "Core/ECS/SpriteAnimator.h"
#include <Core/ECS/RigidBody2D.h>
#include <Core/Event/EventManager.h>
#include <Core/Log.h>
#include <memory>

#include "Components/HealthComponent.h"
#include "Components/HealthIndicator.h"
#include "Components/PointComponent.h"
#include "Components/PointIndicator.h"

#include "Components/LivesIndicator.h"
#include "Components/HighScoreIndicator.h"
#include "Components/Player/SpaceFighter.h"
#include "Components/Enemy/EnemyBug.h"

#include "AnimationStates/FlyInState.h"
#include "AnimationStates/AttackState.h"
#include "AnimationStates/IdleState.h"
#include "AnimationStates/DeathState.h"

#include "Input/MoveCommand.h"
#include "Input/DieCommand.h"
#include "Input/PointCommand.h"
#include "Input/ShootCommand.h"
#include "Input/Utils/SwitchSceneCommand.h"

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

	using Engine::InputState;
	using Engine::InputManager;
	using Engine::SceneManager;

	Galaga::LevelLoader levelLoader{};
	Galaga::GameSettings gameSettings{};

	std::vector<Galaga::Level> levels(3);

	levelLoader.LoadGameSettings("Data/Resources/Saved/game_settings.json", gameSettings);

	for (size_t i{}; i < 1; ++i)
	{
		std::string levelName = std::format("Data/Resources/Levels/lvl_{}.json", i);
		levelLoader.LoadLevel(levelName, levels[i]);
	}

	Galaga::LevelInstancer levelInstancer{ levels, gameSettings, std::make_tuple(windowWidth, windowHeight) };
	///////////////////////////////////////////////////////////////////////////////////////////////////////
	// Input
	///////////////////////////////////////////////////////////////////////////////////////////////////////

	// TODO: improve this
	std::shared_ptr<Engine::GameObject> playerRoot = SceneManager::GetInstance().GetActiveScene()->FindByName("PlayerRoot");

	// Keyboard
	InputManager::GetInstance().AddAxisMapping(SDL_SCANCODE_D, std::make_unique<Galaga::MoveCommand>(playerRoot.get(), glm::vec2(1.0f, 0.0f)));
	InputManager::GetInstance().AddAxisMapping(SDL_SCANCODE_A, std::make_unique<Galaga::MoveCommand>(playerRoot.get(), glm::vec2(-1.0f, 0.0f)));
	InputManager::GetInstance().AddAction(SDL_SCANCODE_SPACE, InputState::Press, std::make_unique<Galaga::ShootCommand>(playerRoot.get()));
	InputManager::GetInstance().AddAction(SDL_SCANCODE_F11, InputState::Press, std::make_unique<Galaga::SwitchSceneCommand>(playerRoot.get()));

	// Controller
	unsigned int controllerIdx = InputManager::GetInstance().AddController();
	InputManager::GetInstance().AddAxisMapping(
		controllerIdx,
		Engine::XboxController::ControllerAxis::LeftThumb,
		std::make_unique<Galaga::MoveCommand>(playerRoot.get())
	);

	InputManager::GetInstance().AddAction(
		controllerIdx,
		Engine::XboxController::ControllerButton::ButtonA,
		InputState::Press,
		std::make_unique<Galaga::ShootCommand>(playerRoot.get())
	);

	InputManager::GetInstance().AddAction(
		controllerIdx,
		Engine::XboxController::ControllerButton::DPadUp,
		InputState::Press,
		std::make_unique<Galaga::SwitchSceneCommand>(playerRoot.get())
	);
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