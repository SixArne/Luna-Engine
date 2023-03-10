#include "pch.h"

#include <SDL.h>

#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include "Engine.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "Scene.h"
#include "GameObject.h"
#include "Core/ECS/TextComponent.h"
#include "Core/ECS/TextureRenderer.h"
#include "Core/ECS/TransformComponent.h"
#include "Core/ECS/Custom/FPSCounter.h"
#include "Core/ECS/Custom/Rotator.h"
#include "Util/OBJParser.h"

void load()
{
	//auto& scene = Engine::SceneManager::GetInstance().CreateScene("Demo");

	//// FPS component
	//auto fpsCounter = std::make_shared<Engine::GameObject>("fpsCounter", glm::vec2{ 500, 0 });
	//fpsCounter->AddComponent<Engine::TextComponent>();
	//fpsCounter->AddComponent<Engine::FPSCounter>();

	//auto root = std::make_shared<Engine::GameObject>("root", glm::vec2{ 100,100 });
	//root->AddComponent<Engine::Rotator>();

	//// Spaces ship
	//auto spaceShip = std::make_shared<Engine::GameObject>("spaceShip", glm::vec2{ 0, 20 });
	//spaceShip->AddComponent<Engine::TextureRendererComponent>("Resources/Sprites/space_ship.png");
	//spaceShip->AddComponent<Engine::Rotator>();

	//// Bug
	//auto bug = std::make_shared<Engine::GameObject>("bug", glm::vec2{0, 20});
	//bug->AddComponent<Engine::TextureRendererComponent>("Resources/Sprites/bug.png");
	//
	//root->AttachChild(spaceShip, false);
	//spaceShip->AttachChild(bug, false);
	//
	//scene.Add(root);

	Util::OBJParser parser{"../Data/Resources/bunny"};
	parser.ReadTextOBJFile();
	parser.WriteToBinary("../Data/Resources/bunny.bobj");

	parser.ReadBinaryOBJFile();
	parser.WriteToText("../Data/Resources/bunny.obj");
	
}

int main(int, char*[]) {
	Engine::Log::Init();

	Engine::Engine engine("../Data/");
	engine.Run(load);
    return 0;
}