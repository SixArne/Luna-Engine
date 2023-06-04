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

#include "AnimationStates/FlyInState.h"
#include "AnimationStates/AttackState.h"
#include "AnimationStates/IdleState.h"
#include "AnimationStates/DeathState.h"

#include <ResourceManager.h>
#include <Core/Input/InputManager.h>
#include <Core/Input/XboxController.h>
#include <Core/ECS/TextureRenderer.h>
#include <Core/ECS/TextComponent.h>
#include "Core/ECS/SpriteAnimator.h"
#include <Core/ECS/RigidBody2D.h>
#include <Core/Event/EventManager.h>
#include <Core/Log.h>

Galaga::LevelInstancer::LevelInstancer(std::vector<Level>& levels, const GameSettings& gameSettings, std::tuple<int, int> windowSize)
    :m_GameSettings{gameSettings}, m_WindowWidth{std::get<0>(windowSize)}, m_WindowHeight{std::get<1>(windowSize)}
{
    for (auto& level: levels)
    {
        CreateLevel(level);
    }
}

void Galaga::LevelInstancer::CreateLevel(Level& level)
{
    Engine::Scene& scene = Engine::SceneManager::GetInstance().CreateScene(level.name);

    scene.Add(CreatePlayer());
    scene.Add(CreateLivesHud());
    scene.Add(CreateHighScoreHud());

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
        enemyRoot->GetTransform()->SetLocalPosition(glm::vec2{ 0.f + offset, 30.f });

		scene.Add(enemyRoot);
    }
}

std::shared_ptr<Engine::GameObject> Galaga::LevelInstancer::CreatePlayer()
{
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
	// Player
	///////////////////////////////////////////////////////////////////////////////////////////////////////
	auto playerRoot = std::make_shared<Engine::GameObject>("PlayerRoot", glm::vec2{ m_WindowWidth / 2, m_WindowHeight - 30 });
	playerRoot->AddComponent<Engine::TextureRendererComponent>("Resources/Sprites/main_shuttle.png");
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

    livesContainer->AttachChild(liveText, false);
	livesContainer->AttachChild(liveImage1, false);
	livesContainer->AttachChild(liveImage2, false);
	livesContainer->AttachChild(liveImage3, false);

    return livesContainer;
}

std::shared_ptr<Engine::GameObject> Galaga::LevelInstancer::CreateBeeEnemy(EnemyTextures& textureInfo)
{
    auto enemyRoot = std::make_shared<Engine::GameObject>("EnemyRoot", glm::vec2{ 0, 0 });
    enemyRoot->AddComponent<Engine::TextureRendererComponent>("Resources/Sprites/bug/idle/1.png");
    Engine::SpriteAnimator* animator = enemyRoot->AddComponent<Engine::SpriteAnimator>();
    enemyRoot->AddComponent<Engine::RigidBody2D>(Engine::RigidBody2DCollider{10,10});
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