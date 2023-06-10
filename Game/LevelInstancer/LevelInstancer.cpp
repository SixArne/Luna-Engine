#include "LevelInstancer.h"
#include <SceneManager.h>
#include <Scene.h>

#include "Components/HealthComponent.h"
#include "Components/HealthIndicator.h"
#include "Components/PointComponent.h"
#include "Components/PointIndicator.h"

#include "Components/LivesIndicator.h"
#include "Components/HighScoreIndicator.h"
#include "Components/Player/SpaceFighter.h"
#include "Components/Enemy/EnemyBug.h"
#include "Components/Menu/OptionContainer.h"
#include "Components/Menu/MenuOption.h"

#include "AnimationStates/FlyInState.h"
#include "AnimationStates/FlyBackState.h"
#include "AnimationStates/AttackState.h"
#include "AnimationStates/IdleState.h"
#include "AnimationStates/DeathState.h"

#include <Core/Input/InputManager.h>

#include "Input/MoveCommand.h"
#include "Input/DieCommand.h"
#include "Input/PointCommand.h"
#include "Input/ShootCommand.h"
#include "Input/Utils/SwitchSceneCommand.h"
#include "Input/Utils/NavigateMenuCommand.h"
#include "Input/Utils/SoundVolumeCommand.h"

#include <ResourceManager.h>
#include <Core/Input/InputManager.h>
#include <Core/Input/XboxController.h>
#include <Core/ECS/TextureRenderer.h>
#include <Core/ECS/TextComponent.h>
#include "Core/ECS/SpriteAnimator.h"
#include <Core/ECS/RigidBody2D.h>
#include <Core/Event/EventManager.h>
#include <Core/Services/ServiceLocator.h>
#include <Core/Services/Physics/PhysicsService.h>
#include <Core/Log.h>
#include <Core/Event/EventManager.h>

#define EVENT(name, level) std::format("{}{}", name, level)

void Galaga::LevelInstancer::Load(std::vector<Level>& levels, const GameSettings& gameSettings, std::tuple<int, int> windowSize)
{
	using Engine::InputManager;
	using Engine::InputState;

	m_GameSettings = gameSettings;
	m_Levels = levels;
	m_WindowWidth = std::get<0>(windowSize);
	m_WindowHeight = std::get<1>(windowSize);

	m_PlayableAreaRangeX = { 50, m_WindowWidth - 100 };
	m_PlayableAreaRangeY = { 0, m_WindowHeight };
	m_MaxEnemiesInRow = 8;
	m_RowHeight = 50;

	// Create menu, players and enemies are created on demand.
	CreateMenu();

	bool controller1Exists = InputManager::GetInstance().HasController(0);
	int controllerIdx{};

	if (controller1Exists)
	{
		controllerIdx = 0;
	}
	else
	{
		controllerIdx = InputManager::GetInstance().AddController();
	}

	auto debugSchema = InputManager::GetInstance().AddSchema("DEBUG_SCHEMA");

	// Scene switching logic
	debugSchema->AddAction(
		controllerIdx,
		Engine::XboxController::ControllerButton::RightThumb,
		InputState::Press,
		std::make_unique<Galaga::SwitchSceneCommand>(nullptr)
	);

	debugSchema->AddAction(SDL_SCANCODE_F11, InputState::Press, std::make_unique<Galaga::SwitchSceneCommand>(nullptr));

	auto audioSchema = InputManager::GetInstance().AddSchema("AUDIO_SCHEMA");
	audioSchema->AddAction(
		controllerIdx,
		Engine::XboxController::ControllerButton::ButtonX,
		InputState::Press,
		std::make_unique<Galaga::SoundVolumeCommand>(nullptr, Galaga::SoundVolumeCommand::Type::Mute)
	);

	auto muteAudioCommand = std::make_unique<Galaga::SoundVolumeCommand>(
		nullptr,
		Galaga::SoundVolumeCommand::Type::Mute
	);

	audioSchema->AddAction(
		SDL_SCANCODE_M,
		InputState::Press,
		std::move(muteAudioCommand)
	);
}

Engine::Scene& Galaga::LevelInstancer::CreateMenu()
{
	using Engine::InputManager;
	using Engine::InputState;

	Engine::Scene* scene = Engine::SceneManager::GetInstance().CreateScene("menu");

	const auto redColor = glm::ivec3{255, 0, 0};

	auto statisticsContainer = std::make_shared<Engine::GameObject>("statistics-container");

	auto hiScore = std::make_shared<Engine::GameObject>("HI-SCORE");

	auto hiScoreText = hiScore->AddComponent<Engine::TextComponent>("HI-SCORE");
	auto hiScoreTextScreenSize = hiScoreText->GetScreenSize();
	hiScoreText->SetColor(redColor);

	hiScore->GetTransform()->AddLocalPosition(
		glm::vec2{
			400,
			50
		}
	);

	auto hiScoreValue = hiScore->AddComponent<Engine::TextComponent>("30000");
	auto hiScoreValueScreenSize = hiScoreText->GetScreenSize();
	hiScoreValue->SetOffset(
		glm::vec2{
			0,
			40
		}
	);

	// Galaga logo
	auto galagaLogo = std::make_shared<Engine::GameObject>("menu_logo");
	auto galagaLogoTexture = galagaLogo->AddComponent<Engine::TextureRenderer>("Resources/Sprites/galaga_logo.png");
	auto galagaLogoScreenSize = galagaLogoTexture->GetScreenSize();
	galagaLogo->GetTransform()->AddLocalPosition(
		glm::vec2{
			0,
			100
		}
	);

	// Menu
	auto menuContainerObject = std::make_shared<Engine::GameObject>("menu-container", glm::vec2{50, 300});
	auto menuContainer = menuContainerObject->AddComponent<Galaga::OptionContainer>();

	// single player option
	auto singlePlayerOptionObject = std::make_shared<Engine::GameObject>("single-player-option", glm::vec2{ 0, 0});
	auto singlePlayerOption = singlePlayerOptionObject->AddComponent<Galaga::MenuOption>(
		"1 PLAYER",
		[this](){
			OnSinglePlayer();
		},
		true
	);

	// coop player option
	auto coopPlayerOptionObject = std::make_shared<Engine::GameObject>("coop-player-option", glm::vec2{ 0, 50});
	auto coopPlayerOption = coopPlayerOptionObject->AddComponent<Galaga::MenuOption>(
		"2 PLAYER",
		[this](){
			OnMultiPlayer();
		},
		false
	);

	// vs player option
	auto vsPlayerOptionObject = std::make_shared<Engine::GameObject>("vs-player-option", glm::vec2{ 0, 100});
	auto vsPlayerOption = vsPlayerOptionObject->AddComponent<Galaga::MenuOption>(
		"VS",
		[this](){
			OnVersus();
		},
		false
	);

	menuContainerObject->AttachChild(singlePlayerOptionObject, false);
	menuContainerObject->AttachChild(coopPlayerOptionObject, false);
	menuContainerObject->AttachChild(vsPlayerOptionObject, false);

	menuContainer->AddOption(singlePlayerOption);
	menuContainer->AddOption(coopPlayerOption);
	menuContainer->AddOption(vsPlayerOption);

	scene->Add(galagaLogo);
	scene->Add(hiScore);
	scene->Add(menuContainerObject);

	// Setup menu input
	auto moveUpCommand = std::make_unique<Galaga::NavigateMenuCommand>(
		menuContainerObject.get(),
		Galaga::NavigateMenuCommandType::Previous
	);

	auto menuSchema = InputManager::GetInstance().AddSchema("MENU_SCHEMA");

	menuSchema->AddAction(
		SDL_SCANCODE_UP,
		InputState::Press,
		std::move(moveUpCommand)
	);

	auto moveDownCommand = std::make_unique<Galaga::NavigateMenuCommand>(
		menuContainerObject.get(),
		Galaga::NavigateMenuCommandType::Next
	);

	menuSchema->AddAction(
		SDL_SCANCODE_DOWN,
		InputState::Press,
		std::move(moveDownCommand)
	);

	auto acceptCommand = std::make_unique<Galaga::NavigateMenuCommand>(
		menuContainerObject.get(),
		Galaga::NavigateMenuCommandType::Click
	);

	menuSchema->AddAction(
		SDL_SCANCODE_RETURN,
		InputState::Press,
		std::move(acceptCommand)
	);

	bool controller1Exists = InputManager::GetInstance().HasController(0);
	int controllerIdx{};

	if (controller1Exists)
	{
		controllerIdx = 0;
	}
	else
	{
		controllerIdx = InputManager::GetInstance().AddController();
	}

	menuSchema->AddAction(
		controllerIdx,
		Engine::XboxController::ControllerButton::DPadUp,
		InputState::Press,
		std::make_unique<Galaga::NavigateMenuCommand>(menuContainerObject.get(), Galaga::NavigateMenuCommandType::Previous)
	);

	menuSchema->AddAction(
		controllerIdx,
		Engine::XboxController::ControllerButton::DPadDown,
		InputState::Press,
		std::make_unique<Galaga::NavigateMenuCommand>(menuContainerObject.get(), Galaga::NavigateMenuCommandType::Next)
	);

	menuSchema->AddAction(
		controllerIdx,
		Engine::XboxController::ControllerButton::ButtonA,
		InputState::Press,
		std::make_unique<Galaga::NavigateMenuCommand>(menuContainerObject.get(), Galaga::NavigateMenuCommandType::Click)
	);

	return *scene;
}

Engine::Scene& Galaga::LevelInstancer::CreateLevel(Level& level)
{
	auto scene = Engine::SceneManager::GetInstance().GetScene(level.name);
	if (scene != nullptr)
	{
		//These 2 calls will block the main thread untill the physics has cleaned up everything.
		scene->Reset();
	}
	else
	{
		scene = Engine::SceneManager::GetInstance().CreateScene(level.name);
	}

	switch (m_GameType)
	{
	case GameType::SinglePlayer:
    	scene->Add(CreateLivesHud("Resources/Sprites/lives.png", glm::vec2{ 450, 400 }, 0));
		break;
	case GameType::MultiPlayer:
    	scene->Add(CreateLivesHud("Resources/Sprites/lives.png", glm::vec2{ 450, 350 }, 0));
    	scene->Add(CreateLivesHud("Resources/Sprites/lives.png", glm::vec2{ 450, 420 }, 1));
		break;
	}


    scene->Add(CreateHighScoreHud(level.name));
	scene->Add(CreateLevelName(level.name));

    // TODO: oberver pattern

    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    // Bees
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
	auto beeIdleTextures = std::vector<std::shared_ptr<Engine::Texture2D>>
	{
		Engine::ResourceManager::GetInstance().LoadTexture("Resources/Sprites/bee/1.png"),
		Engine::ResourceManager::GetInstance().LoadTexture("Resources/Sprites/bee/2.png")
	};

	auto butterflyIdletextures = std::vector<std::shared_ptr<Engine::Texture2D>>
	{
		Engine::ResourceManager::GetInstance().LoadTexture("Resources/Sprites/butterfly/1.png"),
		Engine::ResourceManager::GetInstance().LoadTexture("Resources/Sprites/butterfly/2.png")
	};

	auto bossStage1textures = std::vector<std::shared_ptr<Engine::Texture2D>>
	{
		Engine::ResourceManager::GetInstance().LoadTexture("Resources/Sprites/boss/green/1.png"),
		Engine::ResourceManager::GetInstance().LoadTexture("Resources/Sprites/boss/green/2.png")
	};

	auto bossStage2textures = std::vector<std::shared_ptr<Engine::Texture2D>>
	{
		Engine::ResourceManager::GetInstance().LoadTexture("Resources/Sprites/boss/purple/1.png"),
		Engine::ResourceManager::GetInstance().LoadTexture("Resources/Sprites/boss/purple/2.png")
	};

	auto enemyDeathTextures = std::vector<std::shared_ptr<Engine::Texture2D>>
	{
		Engine::ResourceManager::GetInstance().LoadTexture("Resources/Sprites/enemy_death_sprites/1.png"),
		Engine::ResourceManager::GetInstance().LoadTexture("Resources/Sprites/enemy_death_sprites/2.png"),
		Engine::ResourceManager::GetInstance().LoadTexture("Resources/Sprites/enemy_death_sprites/3.png"),
		Engine::ResourceManager::GetInstance().LoadTexture("Resources/Sprites/enemy_death_sprites/4.png"),
		Engine::ResourceManager::GetInstance().LoadTexture("Resources/Sprites/enemy_death_sprites/5.png"),
	};

    EnemyTextures beeTextures = std::make_tuple(beeIdleTextures, beeIdleTextures, enemyDeathTextures);
	EnemyTextures butterflyTextures = std::make_tuple(butterflyIdletextures, butterflyIdletextures, enemyDeathTextures);
	EnemyTextures bossTextures = std::make_tuple(bossStage1textures, bossStage2textures, enemyDeathTextures);

    // bees
	for (int r{0}; r < level.bee.row_span; ++r)
	{
		for (int c{}; c < m_MaxEnemiesInRow; ++c)
		{
			const float offset = (float)c * (float)level.bee.space_between;

			const auto row = level.bee.row;
			const auto yOffset = row * m_RowHeight + r * m_RowHeight;

			auto enemyRoot = CreateBeeEnemy(beeTextures, glm::vec2{ 80.f + offset, yOffset });

			scene->Add(enemyRoot);
    	}
	}

	for (int r{0}; r < level.butterfly.row_span; ++r)
	{
		for (int c{}; c < m_MaxEnemiesInRow; ++c)
		{
			const float offset = (float)c * (float)level.bee.space_between;

			const auto row = level.butterfly.row;
			const auto yOffset = row * m_RowHeight + r * m_RowHeight;

			auto enemyRoot = CreateButterflyEnemy(butterflyTextures, glm::vec2{ 80.f + offset, yOffset });

			scene->Add(enemyRoot);
		}
	}

	for (int r{0}; r < level.boss.row_span; ++r)
	{
		for (int c{}; c < m_MaxEnemiesInRow; ++c)
		{
			const float offset = (float)c * (float)level.bee.space_between;

			const auto row = level.boss.row;
			const auto yOffset = row * m_RowHeight + r * m_RowHeight;

			auto enemyRoot = CreateBossEnemy(bossTextures, glm::vec2{ 80.f + offset, yOffset });

			scene->Add(enemyRoot);
		}
	}

	return *scene;
}

std::shared_ptr<Engine::GameObject> Galaga::LevelInstancer::CreatePlayer()
{
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
	// Player
	///////////////////////////////////////////////////////////////////////////////////////////////////////
	auto playerRoot = std::make_shared<Engine::GameObject>("PlayerRoot", glm::vec2{ m_WindowWidth / 2, m_WindowHeight - 30 });
	playerRoot->AddComponent<Engine::TextureRenderer>("Resources/Sprites/main_shuttle.png");
	playerRoot->AddComponent<Galaga::SpaceFighter>();
	playerRoot->AddComponent<Engine::RigidBody2D>(Engine::RigidBody2DCollider{10,10});

    return playerRoot;
}

// [[maybe_unused]] required for macro
std::shared_ptr<Engine::GameObject> Galaga::LevelInstancer::CreateHighScoreHud([[maybe_unused]]const std::string& levelName)
{
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
	auto highscoreIndicatorComp = highscoreValue->AddComponent<Galaga::HighscoreIndicator>(10);



	Engine::EventManager::GetInstance().AttachEvent(EVENT("BeeDiedDiving", levelName), [this, highscoreIndicatorComp](Engine::Event*) {
		highscoreIndicatorComp->IncreaseScore(m_GameSettings.points.BEE_DIVING);
	});

	Engine::EventManager::GetInstance().AttachEvent(EVENT("BeeDiedDivingFormation", levelName), [this, highscoreIndicatorComp](Engine::Event*) {
		highscoreIndicatorComp->IncreaseScore(m_GameSettings.points.BEE_IN_FORMATION);
	});

    highscoreIndented->AttachChild(highscoreText, false);
	highscoreIndented->AttachChild(highscoreValue, false);
	highscoreContainer->AttachChild(highscoreIndented, false);

    return highscoreContainer;
}

std::shared_ptr<Engine::GameObject> Galaga::LevelInstancer::CreateLivesHud(const std::string& liveSpritePath, glm::vec2 position, int playerIndex)
{
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
	// Lives
	///////////////////////////////////////////////////////////////////////////////////////////////////////
	auto livesContainer = std::make_shared<Engine::GameObject>("LivesContainer", position);
	std::vector<Engine::TextureRenderer*> lifeTextures{};

	auto liveText = std::make_shared<Engine::GameObject>("LiveText", glm::vec2{ 0, 0 });
	liveText->AddComponent<Engine::TextComponent>("LIVES");

	auto liveImage1 = std::make_shared<Engine::GameObject>("LiveImage1", glm::vec2{ 0, 40 });
	lifeTextures.push_back(liveImage1->AddComponent<Engine::TextureRenderer>(liveSpritePath));

	auto liveImage2 = std::make_shared<Engine::GameObject>("LiveImage1", glm::vec2{ 20, 40 });
	lifeTextures.push_back(liveImage2->AddComponent<Engine::TextureRenderer>(liveSpritePath));

	auto liveImage3 = std::make_shared<Engine::GameObject>("LiveImage1", glm::vec2{ 40, 40 });
	lifeTextures.push_back(liveImage3->AddComponent<Engine::TextureRenderer>(liveSpritePath));

	auto livesIndicatorComp = livesContainer->AddComponent<Galaga::LivesIndicator>(lifeTextures);
	livesContainer->SetShouldRenderImGui(true);

    livesContainer->AttachChild(liveText, false);
	livesContainer->AttachChild(liveImage1, false);
	livesContainer->AttachChild(liveImage2, false);
	livesContainer->AttachChild(liveImage3, false);

	m_Players[playerIndex]->GetComponent<Galaga::SpaceFighter>()->RegisterObserver(livesIndicatorComp);

    return livesContainer;
}

std::shared_ptr<Engine::GameObject> Galaga::LevelInstancer::CreateLevelName(const std::string& levelName)
{
	///////////////////////////////////////////////////////////////////////////////////////////////////////
	// Level name
	///////////////////////////////////////////////////////////////////////////////////////////////////////
	auto highscoreText = std::make_shared<Engine::GameObject>("HighscoreText", glm::vec2{ 450, 250 });
	highscoreText->AddComponent<Engine::TextComponent>(levelName);

    return highscoreText;
}

std::shared_ptr<Engine::GameObject> Galaga::LevelInstancer::CreateBeeEnemy(EnemyTextures& textureInfo, glm::vec2 targetPos)
{
    auto enemyRoot = std::make_shared<Engine::GameObject>("EnemyRoot", glm::vec2{ 0, 0 });
    enemyRoot->AddComponent<Engine::TextureRenderer>("Resources/Sprites/bee/1.png");
    Engine::SpriteAnimator* animator = enemyRoot->AddComponent<Engine::SpriteAnimator>();
    enemyRoot->AddComponent<Engine::RigidBody2D>(Engine::RigidBody2DCollider{30,24});
    enemyRoot->AddComponent<Galaga::EnemyBug>(targetPos);
	enemyRoot->AddTag("enemy");

    // Add animations
    animator->AddState<Galaga::FlyInState>("fly_in");

    auto enemyAttack = animator->AddState<Galaga::AttackState>("attack");
    enemyAttack->SetTextures(std::get<0>(textureInfo));

    auto enemyIdle = animator->AddState<Galaga::IdleState>("idle");
    enemyIdle->SetTextures(std::get<1>(textureInfo));

	auto enemyFlyBack = animator->AddState<Galaga::FlyBackState>("flyBack");
    enemyFlyBack->SetTextures(std::get<1>(textureInfo));

    auto enemyDeath = animator->AddState<Galaga::DeathState>("death");
    enemyDeath->SetTextures(std::get<2>(textureInfo));

    return enemyRoot;
}

std::shared_ptr<Engine::GameObject> Galaga::LevelInstancer::CreateButterflyEnemy(EnemyTextures& textureInfo, glm::vec2 targetPos)
{
	auto enemyRoot = std::make_shared<Engine::GameObject>("EnemyRoot", glm::vec2{ 0, 0 });
    enemyRoot->AddComponent<Engine::TextureRenderer>("Resources/Sprites/butterfly/1.png");
    Engine::SpriteAnimator* animator = enemyRoot->AddComponent<Engine::SpriteAnimator>();
    enemyRoot->AddComponent<Engine::RigidBody2D>(Engine::RigidBody2DCollider{30,24});
    enemyRoot->AddComponent<Galaga::EnemyBug>(targetPos);
	enemyRoot->AddTag("enemy");

    // Add animations
    animator->AddState<Galaga::FlyInState>("fly_in");

    auto enemyAttack = animator->AddState<Galaga::AttackState>("attack");
    enemyAttack->SetTextures(std::get<0>(textureInfo));

    auto enemyIdle = animator->AddState<Galaga::IdleState>("idle");
    enemyIdle->SetTextures(std::get<1>(textureInfo));

	auto enemyFlyBack = animator->AddState<Galaga::FlyBackState>("flyBack");
    enemyFlyBack->SetTextures(std::get<1>(textureInfo));

    auto enemyDeath = animator->AddState<Galaga::DeathState>("death");
    enemyDeath->SetTextures(std::get<2>(textureInfo));

    return enemyRoot;
}

std::shared_ptr<Engine::GameObject> Galaga::LevelInstancer::CreateBossEnemy(EnemyTextures& textureInfo, glm::vec2 targetPos)
{
	auto enemyRoot = std::make_shared<Engine::GameObject>("EnemyRoot", glm::vec2{ 0, 0 });
    enemyRoot->AddComponent<Engine::TextureRenderer>("Resources/Sprites/boss/green/1.png");
    Engine::SpriteAnimator* animator = enemyRoot->AddComponent<Engine::SpriteAnimator>();
    enemyRoot->AddComponent<Engine::RigidBody2D>(Engine::RigidBody2DCollider{30,24});
    enemyRoot->AddComponent<Galaga::EnemyBug>(targetPos);
	enemyRoot->AddTag("enemy");

    // Add animations
    animator->AddState<Galaga::FlyInState>("fly_in");

    auto enemyAttack = animator->AddState<Galaga::AttackState>("attack");
    enemyAttack->SetTextures(std::get<0>(textureInfo));

    auto enemyIdle = animator->AddState<Galaga::IdleState>("idle");
    enemyIdle->SetTextures(std::get<1>(textureInfo));

	auto enemyFlyBack = animator->AddState<Galaga::FlyBackState>("flyBack");
    enemyFlyBack->SetTextures(std::get<1>(textureInfo));

    auto enemyDeath = animator->AddState<Galaga::DeathState>("death");
    enemyDeath->SetTextures(std::get<2>(textureInfo));

    return enemyRoot;
}

void Galaga::LevelInstancer::CreateLevels()
{
	for (auto& level : m_Levels)
	{
		CreateLevel(level);
	}
}

void Galaga::LevelInstancer::OnSinglePlayer()
{
	m_GameType = GameType::SinglePlayer;

	using Engine::InputManager;
	using Engine::InputState;

	// player will be persistant throughout l
	m_Players.clear();
	m_Players.emplace_back(CreatePlayer());

	// setup single player here
	// All level loading and creation should be moves here
	CreateLevels();

	auto playerOne = m_Players[0];
	Engine::SceneManager::GetInstance().GetSceneByIndex(1)->Add(playerOne, true);

	auto gameSchema = InputManager::GetInstance().GetSchema("GAME_SCHEMA");
	if (gameSchema != nullptr) // This means that the level was already loaded
	{
		gameSchema->Clear();
	}
	else
	{
		gameSchema = InputManager::GetInstance().AddSchema("GAME_SCHEMA");
	}

	// Keyboard
	gameSchema->AddAxisMapping(SDL_SCANCODE_D, std::make_unique<Galaga::MoveCommand>(playerOne.get(), glm::vec2(1.0f, 0.0f)));
	gameSchema->AddAxisMapping(SDL_SCANCODE_A, std::make_unique<Galaga::MoveCommand>(playerOne.get(), glm::vec2(-1.0f, 0.0f)));
	gameSchema->AddAction(SDL_SCANCODE_SPACE, InputState::Press, std::make_unique<Galaga::ShootCommand>(playerOne.get()));

	// Controller
	bool controller1Exists = InputManager::GetInstance().HasController(0);
	int controllerIdx{};

	if (controller1Exists)
	{
		controllerIdx = 0;
	}
	else
	{
		controllerIdx = InputManager::GetInstance().AddController();
	}

	gameSchema->AddAxisMapping(
		controllerIdx,
		Engine::XboxController::ControllerAxis::LeftThumb,
		std::make_unique<Galaga::MoveCommand>(playerOne.get())
	);

	gameSchema->AddAction(
		controllerIdx,
		Engine::XboxController::ControllerButton::ButtonA,
		InputState::Press,
		std::make_unique<Galaga::ShootCommand>(playerOne.get())
	);

	// Add player to scene
	Engine::SceneManager::GetInstance().GetNextScene();
}

void Galaga::LevelInstancer::OnMultiPlayer()
{
	m_GameType = GameType::MultiPlayer;

	using Engine::InputManager;
	using Engine::InputState;

	m_Players.clear();
	// player 0
	m_Players.emplace_back(CreatePlayer());
	// player 1
	m_Players.emplace_back(CreatePlayer());

	// TODO: Check if scene already exists and clear it if it does

	CreateLevels();
	// setup multiplayer here

	auto playerOne = m_Players[0];
	auto playerTwo = m_Players[1];

	Engine::SceneManager::GetInstance().GetSceneByIndex(1)->Add(playerOne, true);
	Engine::SceneManager::GetInstance().GetSceneByIndex(1)->Add(playerTwo, true);

	auto gameSchema = InputManager::GetInstance().GetSchema("GAME_SCHEMA");
	if (gameSchema != nullptr) // This means that the level was already loaded
	{
		gameSchema->Clear();
	}
	else
	{
		gameSchema = InputManager::GetInstance().AddSchema("GAME_SCHEMA");
	}


	// Keyboard
	gameSchema->AddAxisMapping(SDL_SCANCODE_D, std::make_unique<Galaga::MoveCommand>(playerOne.get(), glm::vec2(1.0f, 0.0f)));
	gameSchema->AddAxisMapping(SDL_SCANCODE_A, std::make_unique<Galaga::MoveCommand>(playerOne.get(), glm::vec2(-1.0f, 0.0f)));
	gameSchema->AddAction(SDL_SCANCODE_SPACE, InputState::Press, std::make_unique<Galaga::ShootCommand>(playerOne.get()));

	// Controller
	bool controller1Exists = InputManager::GetInstance().HasController(0);
	int controllerIdx{};

	if (controller1Exists)
	{
		controllerIdx = 0;
	}
	else
	{
		controllerIdx = InputManager::GetInstance().AddController();
	}

	gameSchema->AddAxisMapping(
		controllerIdx,
		Engine::XboxController::ControllerAxis::LeftThumb,
		std::make_unique<Galaga::MoveCommand>(playerTwo.get())
	);

	gameSchema->AddAction(
		controllerIdx,
		Engine::XboxController::ControllerButton::ButtonA,
		InputState::Press,
		std::make_unique<Galaga::ShootCommand>(playerTwo.get())
	);

	Engine::SceneManager::GetInstance().GetNextScene();
}

void Galaga::LevelInstancer::OnVersus()
{
	L_DEBUG("Selected versus");

	// setup versus here
}