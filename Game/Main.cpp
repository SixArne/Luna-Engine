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

#include "Input/MoveCommand.h"
#include "Input/DieCommand.h"
#include "Input/PointCommand.h"
#include "Input/ShootCommand.h"

#include <Core/Services/ServiceLocator.h>
#include <Core/Services/Sound/LoggingSoundSystem.h>
#include <Core/Services/Sound/SDLSoundSystem.h>

void load()
{
	constexpr auto windowWidth = 640;
	constexpr auto windowHeight = 480;

	using Engine::InputState;
	using Engine::InputManager;
	using Engine::SceneManager;


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

	Engine::Log::Init();

	#ifdef DEBUG
	Engine::ServiceLocator::RegisterSoundService(new LoggingSoundSystem(new SDLSoundSystem(4)));
	#else
	Engine::ServiceLocator::RegisterSoundService(new SDLSoundSystem(4));
	#endif

	auto cwd = std::filesystem::current_path();

	L_DEBUG("Current path: {0}", cwd.string());

	Engine::Engine engine("./Data/");
	engine.Run(load);

	Engine::ServiceLocator::DestroyServices();

	return 0;
}