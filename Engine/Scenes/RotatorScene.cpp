#include "pch.h"
#include "RotatorScene.h"
#include "imgui.h"
#include "Core/ECS/Custom/Rotator.h"
#include "Core/ECS/TextureRenderer.h"
#include "GameObject.h"

Engine::RotatorScene::RotatorScene(const std::string& scenename)
	:Scene(scenename)
{
	// init scene here

	auto root = std::make_shared<Engine::GameObject>("root", glm::vec2{ 100,100 });
	root->AddComponent<Engine::Rotator>();

	// Spaces ship
	auto spaceShip = std::make_shared<Engine::GameObject>("spaceShip", glm::vec2{ 0, 20 });
	spaceShip->AddComponent<Engine::TextureRendererComponent>("Resources/Sprites/space_ship.png");
	spaceShip->AddComponent<Engine::Rotator>();

	// Bug
	auto bug = std::make_shared<Engine::GameObject>("bug", glm::vec2{ 0, 20 });
	bug->AddComponent<Engine::TextureRendererComponent>("Resources/Sprites/bug.png");

	root->AttachChild(spaceShip, false);
	spaceShip->AttachChild(bug, false);

	Add(root);
}

void Engine::RotatorScene::OnImGui()
{
	ImGui::Begin("Debug params");
	ImGui::Text("hiii");
	auto loc = m_objects[0]->GetTransform()->GetLocalPosition();
	ImGui::SliderFloat2("Root position", (float*) & loc, 0.f, 200.f);
	m_objects[0]->GetTransform()->SetLocalPosition(loc);
	ImGui::End();
}
