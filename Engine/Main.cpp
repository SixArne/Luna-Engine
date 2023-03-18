#include "pch.h"
#define SDL_MAIN_HANDLED

#include <imgui.h>
#include <implot.h>
#include <glm/glm.hpp>
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
#include "Scenes/RotatorScene.h"

#include "Util/OBJParser.h"


void load()
{
	auto rotatorScene = std::make_shared<Engine::RotatorScene>("RotatorScene");

	Engine::SceneManager::GetInstance().AddScene(rotatorScene);
}

int main(int, char*[]) {
	Engine::Log::Init();
	int test{};
	Engine::Engine engine("../Data/");
	engine.Run(load);
    return 0;
}