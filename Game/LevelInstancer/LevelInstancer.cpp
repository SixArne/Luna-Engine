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

#include <ResourceManager.h>
#include <Core/Input/InputManager.h>
#include <Core/Input/XboxController.h>
#include <Core/ECS/TextureRenderer.h>
#include <Core/ECS/TextComponent.h>
#include "Core/ECS/SpriteAnimator.h"
#include <Core/ECS/RigidBody2D.h>
#include <Core/Event/EventManager.h>
#include <Core/Log.h>

void Galaga::LevelInstancer::Load(std::vector<Level>& levels, const GameSettings& gameSettings, std::tuple<int, int> windowSize)
{
	m_GameSettings = gameSettings;
	m_WindowWidth = std::get<0>(windowSize);
	m_WindowHeight = std::get<1>(windowSize);

	using Engine::InputManager;
	using Engine::InputState;

	// player will be persistant throughout levels
	m_Player = CreatePlayer();

	CreateMenu();

	for (auto& level : levels)
	{
		CreateLevel(level);
	}

	// scene 1 is first level, 0 is menu
	Engine::SceneManager::GetInstance().GetSceneByIndex(1)->Add(m_Player, true);

	// Keyboard
	InputManager::GetInstance().AddAxisMapping(SDL_SCANCODE_D, std::make_unique<Galaga::MoveCommand>(m_Player.get(), glm::vec2(1.0f, 0.0f)));
	InputManager::GetInstance().AddAxisMapping(SDL_SCANCODE_A, std::make_unique<Galaga::MoveCommand>(m_Player.get(), glm::vec2(-1.0f, 0.0f)));
	InputManager::GetInstance().AddAction(SDL_SCANCODE_SPACE, InputState::Press, std::make_unique<Galaga::ShootCommand>(m_Player.get()));
	InputManager::GetInstance().AddAction(SDL_SCANCODE_F11, InputState::Press, std::make_unique<Galaga::SwitchSceneCommand>(m_Player.get()));

	// Controller
	unsigned int controllerIdx = InputManager::GetInstance().AddController();
	InputManager::GetInstance().AddAxisMapping(
		controllerIdx,
		Engine::XboxController::ControllerAxis::LeftThumb,
		std::make_unique<Galaga::MoveCommand>(m_Player.get())
	);

	InputManager::GetInstance().AddAction(
		controllerIdx,
		Engine::XboxController::ControllerButton::ButtonA,
		InputState::Press,
		std::make_unique<Galaga::ShootCommand>(m_Player.get())
	);

	InputManager::GetInstance().AddAction(
		controllerIdx,
		Engine::XboxController::ControllerButton::RightThumb,
		InputState::Press,
		std::make_unique<Galaga::SwitchSceneCommand>(m_Player.get())
	);
}

Engine::Scene& Galaga::LevelInstancer::CreateMenu()
{
	using Engine::InputManager;
	using Engine::InputState;

	Engine::Scene& scene = Engine::SceneManager::GetInstance().CreateScene("menu");

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

	scene.Add(galagaLogo);
	scene.Add(hiScore);
	scene.Add(menuContainerObject);

	// Setup menu input
	auto moveUpCommand = std::make_unique<Galaga::NavigateMenuCommand>(
		menuContainerObject.get(),
		Galaga::NavigateMenuCommandType::Previous
	);

	InputManager::GetInstance().AddAction(
		SDL_SCANCODE_UP,
		InputState::Press,
		std::move(moveUpCommand)
	);

	auto moveDownCommand = std::make_unique<Galaga::NavigateMenuCommand>(
		menuContainerObject.get(),
		Galaga::NavigateMenuCommandType::Next
	);

	InputManager::GetInstance().AddAction(
		SDL_SCANCODE_DOWN,
		InputState::Press,
		std::move(moveDownCommand)
	);

	auto acceptCommand = std::make_unique<Galaga::NavigateMenuCommand>(
		menuContainerObject.get(),
		Galaga::NavigateMenuCommandType::Click
	);

	InputManager::GetInstance().AddAction(
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

	InputManager::GetInstance().AddAction(
		controllerIdx,
		Engine::XboxController::ControllerButton::DPadUp,
		InputState::Press,
		std::make_unique<Galaga::NavigateMenuCommand>(menuContainerObject.get(), Galaga::NavigateMenuCommandType::Previous)
	);

	InputManager::GetInstance().AddAction(
		controllerIdx,
		Engine::XboxController::ControllerButton::DPadDown,
		InputState::Press,
		std::make_unique<Galaga::NavigateMenuCommand>(menuContainerObject.get(), Galaga::NavigateMenuCommandType::Next)
	);

	InputManager::GetInstance().AddAction(
		controllerIdx,
		Engine::XboxController::ControllerButton::ButtonA,
		InputState::Press,
		std::make_unique<Galaga::NavigateMenuCommand>(menuContainerObject.get(), Galaga::NavigateMenuCommandType::Click)
	);

	return scene;
}

Engine::Scene& Galaga::LevelInstancer::CreateLevel(Level& level)
{
    Engine::Scene& scene = Engine::SceneManager::GetInstance().CreateScene(level.name);

    scene.Add(CreateLivesHud());
    scene.Add(CreateHighScoreHud());
	scene.Add(CreateLevelName(level.name));

    // TODO: oberver pattern

    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    // Bees
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    auto beeAttackTextures = std::vector<std::shared_ptr<Engine::Texture2D>>
	{
		Engine::ResourceManager::GetInstance().LoadTexture("Resources/Sprites/zako/1.png"),
		Engine::ResourceManager::GetInstance().LoadTexture("Resources/Sprites/zako/2.png")
	};

	auto beeIdleTextures = std::vector<std::shared_ptr<Engine::Texture2D>>
	{
		Engine::ResourceManager::GetInstance().LoadTexture("Resources/Sprites/zako/1.png"),
		Engine::ResourceManager::GetInstance().LoadTexture("Resources/Sprites/zako/2.png")
	};

	auto enemyDeathTextures = std::vector<std::shared_ptr<Engine::Texture2D>>
	{
		Engine::ResourceManager::GetInstance().LoadTexture("Resources/Sprites/enemy_death_sprites/1.png"),
		Engine::ResourceManager::GetInstance().LoadTexture("Resources/Sprites/enemy_death_sprites/2.png"),
		Engine::ResourceManager::GetInstance().LoadTexture("Resources/Sprites/enemy_death_sprites/3.png"),
		Engine::ResourceManager::GetInstance().LoadTexture("Resources/Sprites/enemy_death_sprites/4.png"),
		Engine::ResourceManager::GetInstance().LoadTexture("Resources/Sprites/enemy_death_sprites/5.png"),
	};

    EnemyTextures beeTextures = std::make_tuple(beeAttackTextures, beeIdleTextures, enemyDeathTextures);

    // bees
    for (int i{}; i < level.bee.count; ++i)
    {
        const float offset = (float)i * (float)level.bee.space_between;

        auto enemyRoot = CreateBeeEnemy(beeTextures);
        enemyRoot->GetTransform()->SetLocalPosition(glm::vec2{ 80.f + offset, 30.f });

		scene.Add(enemyRoot);
    }

	return scene;
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

std::shared_ptr<Engine::GameObject> Galaga::LevelInstancer::CreateHighScoreHud()
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
	/*auto highscoreIndicatorComp = */highscoreValue->AddComponent<Galaga::HighscoreIndicator>(0);

    highscoreIndented->AttachChild(highscoreText, false);
	highscoreIndented->AttachChild(highscoreValue, false);
	highscoreContainer->AttachChild(highscoreIndented, false);

    return highscoreContainer;
}

std::shared_ptr<Engine::GameObject> Galaga::LevelInstancer::CreateLivesHud()
{
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
	// Lives
	///////////////////////////////////////////////////////////////////////////////////////////////////////
	auto livesContainer = std::make_shared<Engine::GameObject>("LivesContainer", glm::vec2{ 450, 400 });
	std::vector<Engine::TextureRenderer*> lifeTextures{};

	auto liveText = std::make_shared<Engine::GameObject>("LiveText", glm::vec2{ 0, 0 });
	liveText->AddComponent<Engine::TextComponent>("LIVES");

	auto liveImage1 = std::make_shared<Engine::GameObject>("LiveImage1", glm::vec2{ 0, 40 });
	lifeTextures.push_back(liveImage1->AddComponent<Engine::TextureRenderer>("Resources/Sprites/lives.png"));

	auto liveImage2 = std::make_shared<Engine::GameObject>("LiveImage1", glm::vec2{ 20, 40 });
	lifeTextures.push_back(liveImage2->AddComponent<Engine::TextureRenderer>("Resources/Sprites/lives.png"));

	auto liveImage3 = std::make_shared<Engine::GameObject>("LiveImage1", glm::vec2{ 40, 40 });
	lifeTextures.push_back(liveImage3->AddComponent<Engine::TextureRenderer>("Resources/Sprites/lives.png"));

	livesContainer->AddComponent<Galaga::LivesIndicator>(lifeTextures);
	livesContainer->SetShouldRenderImGui(true);

    livesContainer->AttachChild(liveText, false);
	livesContainer->AttachChild(liveImage1, false);
	livesContainer->AttachChild(liveImage2, false);
	livesContainer->AttachChild(liveImage3, false);

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

std::shared_ptr<Engine::GameObject> Galaga::LevelInstancer::CreateBeeEnemy(EnemyTextures& textureInfo)
{
    auto enemyRoot = std::make_shared<Engine::GameObject>("EnemyRoot", glm::vec2{ 0, 0 });
    enemyRoot->AddComponent<Engine::TextureRenderer>("Resources/Sprites/zako/1.png");
    Engine::SpriteAnimator* animator = enemyRoot->AddComponent<Engine::SpriteAnimator>();
    enemyRoot->AddComponent<Engine::RigidBody2D>(Engine::RigidBody2DCollider{30,24});
    enemyRoot->AddComponent<Galaga::EnemyBug>();

    // Add animations
    animator->AddState<Galaga::FlyInState>("fly_in");

    auto enemyAttack = animator->AddState<Galaga::AttackState>("attack");
    enemyAttack->SetTextures(std::get<0>(textureInfo));

    auto enemyIdle = animator->AddState<Galaga::IdleState>("idle");
    enemyIdle->SetTextures(std::get<1>(textureInfo));

    auto enemyDeath = animator->AddState<Galaga::DeathState>("death");
    enemyDeath->SetTextures(std::get<2>(textureInfo));

    return enemyRoot;
}

// std::shared_ptr<Engine::GameObject> Galaga::LevelInstancer::CreateButterflyEnemy(EnemyTextures& textureInfo)
// {

// }

// std::shared_ptr<Engine::GameObject> Galaga::LevelInstancer::CreateBossEnemy(EnemyTextures& textureInfo)
// {

// }

void Galaga::LevelInstancer::OnSinglePlayer()
{
	L_DEBUG("Selected singleplayer ");

	// setup single player here
	// All level loading and creation should be moves here
}

void Galaga::LevelInstancer::OnMultiPlayer()
{
	L_DEBUG("Selected multiplayer");

	// setup multiplayer here
}

void Galaga::LevelInstancer::OnVersus()
{
	L_DEBUG("Selected versus");

	// setup versus here
}