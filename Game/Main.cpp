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

#include <Core/Services/ServiceLocator.h>
#include <Core/Services/Sound/LoggingSoundSystem.h>
#include <Core/Services/Sound/SDLSoundSystem.h>
#include <Core/Services/Physics/PhysicsService.h>

#include "LevelLoader/LevelLoader.h"

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

	for (size_t i{}; i < levels.size(); ++i)
	{
		std::string levelName = std::format("Data/Resources/Levels/lvl_{}.json", i);
		levelLoader.LoadLevel(levelName, levels[i]);
	}

	auto& scene = SceneManager::GetInstance().CreateScene("Game");

	///////////////////////////////////////////////////////////////////////////////////////////////////////
	// HighScore
	///////////////////////////////////////////////////////////////////////////////////////////////////////
	auto highscoreContainer = std::make_shared<Engine::GameObject>("HighscoreContainer", glm::vec2{ 450, 0 });
	highscoreContainer->AddComponent<Engine::TextComponent>("HIGH");

	auto highscoreIndented = std::make_shared<Engine::GameObject>("HighscoreIndent", glm::vec2{ 20, 50 });

	auto highscoreText = std::make_shared<Engine::GameObject>("HighscoreText", glm::vec2{ 0, 0 });
	highscoreText->AddComponent<Engine::TextComponent>("SCORE");

	auto highscoreValue = std::make_shared<Engine::GameObject>("HighscoreValue", glm::vec2{ 0, 50 });
	highscoreValue->AddComponent<Engine::TextComponent>("0");
	/*auto highscoreIndicatorComp = */highscoreValue->AddComponent<Galaga::HighscoreIndicator>(0);


	///////////////////////////////////////////////////////////////////////////////////////////////////////
	// Lives
	///////////////////////////////////////////////////////////////////////////////////////////////////////
	auto livesContainer = std::make_shared<Engine::GameObject>("LivesContainer", glm::vec2{ 450, 400 });
	std::vector<Engine::TextureRendererComponent*> lifeTextures{};

	auto liveText = std::make_shared<Engine::GameObject>("LiveText", glm::vec2{ 0, 0 });
	liveText->AddComponent<Engine::TextComponent>("LIVES");

	auto liveImage1 = std::make_shared<Engine::GameObject>("LiveImage1", glm::vec2{ 0, 40 });
	lifeTextures.push_back(liveImage1->AddComponent<Engine::TextureRendererComponent>("Resources/Sprites/lives.png"));

	auto liveImage2 = std::make_shared<Engine::GameObject>("LiveImage1", glm::vec2{ 20, 40 });
	lifeTextures.push_back(liveImage2->AddComponent<Engine::TextureRendererComponent>("Resources/Sprites/lives.png"));

	auto liveImage3 = std::make_shared<Engine::GameObject>("LiveImage1", glm::vec2{ 40, 40 });
	lifeTextures.push_back(liveImage3->AddComponent<Engine::TextureRendererComponent>("Resources/Sprites/lives.png"));

	livesContainer->AddComponent<Galaga::LivesIndicator>(lifeTextures);
	livesContainer->SetShouldRenderImGui(true);

	///////////////////////////////////////////////////////////////////////////////////////////////////////
	// Player
	///////////////////////////////////////////////////////////////////////////////////////////////////////
	auto playerRoot = std::make_shared<Engine::GameObject>("PlayerRoot", glm::vec2{ windowWidth / 2, windowHeight - 30 });
	playerRoot->AddComponent<Engine::TextureRendererComponent>("Resources/Sprites/main_shuttle.png");
	playerRoot->AddComponent<Galaga::SpaceFighter>();
	playerRoot->AddComponent<Engine::RigidBody2D>(Engine::RigidBody2DCollider{10,10});

	///////////////////////////////////////////////////////////////////////////////////////////////////////
	// Enemy
	///////////////////////////////////////////////////////////////////////////////////////////////////////

	auto bugAttackTextures = std::vector<std::shared_ptr<Engine::Texture2D>>
	{
		Engine::ResourceManager::GetInstance().LoadTexture("Resources/Sprites/zako/1.png"),
		Engine::ResourceManager::GetInstance().LoadTexture("Resources/Sprites/zako/2.png")
	};

	auto bugIdleTextures = std::vector<std::shared_ptr<Engine::Texture2D>>
	{
		Engine::ResourceManager::GetInstance().LoadTexture("Resources/Sprites/zako/1.png"),
		Engine::ResourceManager::GetInstance().LoadTexture("Resources/Sprites/zako/2.png")
	};

	auto bugDeathTextures = std::vector<std::shared_ptr<Engine::Texture2D>>
	{
		Engine::ResourceManager::GetInstance().LoadTexture("Resources/Sprites/enemy_death_sprites/1.png"),
		Engine::ResourceManager::GetInstance().LoadTexture("Resources/Sprites/enemy_death_sprites/2.png"),
		Engine::ResourceManager::GetInstance().LoadTexture("Resources/Sprites/enemy_death_sprites/3.png"),
		Engine::ResourceManager::GetInstance().LoadTexture("Resources/Sprites/enemy_death_sprites/4.png"),
		Engine::ResourceManager::GetInstance().LoadTexture("Resources/Sprites/enemy_death_sprites/5.png"),
	};

	for (int i {}; i < 10; i++)
	{
		auto enemyRoot = std::make_shared<Engine::GameObject>("EnemyRoot", glm::vec2{ 20 + (i * 25), 30 });
		enemyRoot->AddComponent<Engine::TextureRendererComponent>("Resources/Sprites/bug/idle/1.png");
		Engine::SpriteAnimator* animator = enemyRoot->AddComponent<Engine::SpriteAnimator>();
		enemyRoot->AddComponent<Engine::RigidBody2D>(Engine::RigidBody2DCollider{10,10});
		enemyRoot->AddComponent<Galaga::EnemyBug>();

		// Add animations
		animator->AddState<Galaga::FlyInState>("fly_in");

		auto enemyAttack = animator->AddState<Galaga::AttackState>("attack");
		enemyAttack->SetTextures(bugAttackTextures);

		auto enemyIdle = animator->AddState<Galaga::IdleState>("idle");
		enemyIdle->SetTextures(bugIdleTextures);

		auto enemyDeath = animator->AddState<Galaga::DeathState>("death");
		enemyDeath->SetTextures(bugDeathTextures);

		scene.Add(enemyRoot);
	}


	///////////////////////////////////////////////////////////////////////////////////////////////////////
	// Attachments
	///////////////////////////////////////////////////////////////////////////////////////////////////////
	highscoreIndented->AttachChild(highscoreText, false);
	highscoreIndented->AttachChild(highscoreValue, false);
	highscoreContainer->AttachChild(highscoreIndented, false);

	livesContainer->AttachChild(liveText, false);
	livesContainer->AttachChild(liveImage1, false);
	livesContainer->AttachChild(liveImage2, false);
	livesContainer->AttachChild(liveImage3, false);

	///////////////////////////////////////////////////////////////////////////////////////////////////////
	// Adding to scene
	///////////////////////////////////////////////////////////////////////////////////////////////////////
	scene.Add(highscoreContainer);
	scene.Add(livesContainer);
	scene.Add(playerRoot);

	///////////////////////////////////////////////////////////////////////////////////////////////////////
	// Input
	///////////////////////////////////////////////////////////////////////////////////////////////////////

	// Keyboard
	InputManager::GetInstance().AddAxisMapping(SDL_SCANCODE_D, std::make_unique<Galaga::MoveCommand>(playerRoot.get(), glm::vec2(1.0f, 0.0f)));
	InputManager::GetInstance().AddAxisMapping(SDL_SCANCODE_A, std::make_unique<Galaga::MoveCommand>(playerRoot.get(), glm::vec2(-1.0f, 0.0f)));
	InputManager::GetInstance().AddAction(SDL_SCANCODE_SPACE, InputState::Press, std::make_unique<Galaga::ShootCommand>(playerRoot.get()));

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