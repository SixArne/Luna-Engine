#include "pch.h"

#include <SDL.h>
#include "InputManager.h"
#include "imgui_impl_sdl2.h"
#include <Xinput.h>
#include "Core/Input/XboxController.h"

bool Engine::InputManager::ProcessInput()
{

	SDL_Event e;

	// This checks and handles console input
	UpdateConsoleInput();

	while (SDL_PollEvent(&e)) {
		if (e.type == SDL_QUIT) {
			return false;
		}

		UpdateComputerInput(e);

		// Process Imgui event.
		ImGui_ImplSDL2_ProcessEvent(&e);

		// Exit for Imgui viewports.
		if (e.type == SDL_WINDOWEVENT && e.window.event == SDL_WINDOWEVENT_CLOSE)
			return false;
	}


	// Specifically for holding a key
	// #TODO: Figure out how to optimize this
	int keys{};
	auto state = SDL_GetKeyboardState(&keys);
	for (auto& command : m_DesktopCommands)
	{
		// #TODO add list of dpad keys to ignore. 
		if (state[command.first] && command.second.first == InputState::Hold)
		{
			command.second.second->Execute();
		}
	}

	return true;
}

Engine::XboxController* Engine::InputManager::GetController() const
{
	return m_XBoxController.get();
}

void Engine::InputManager::CreateController(int controllerIndex)
{
	m_XBoxController = std::make_unique<XboxController>(controllerIndex);
}

void Engine::InputManager::UpdateComputerInput(SDL_Event event)
{
	//int keys{};
	//auto state = SDL_GetKeyboardState(&keys);

	for (auto& command : m_DesktopCommands)
	{
		auto key = event.key.keysym.scancode;

		if (command.first != key)
			continue;

		// first check hold
		

		// Check press or release
		switch (event.type)
		{
		case SDL_KEYUP:
			if (command.second.first == InputState::Release)
			{
				command.second.second->Execute();
			}
			break;

		case SDL_KEYDOWN:
			if (command.second.first == InputState::Press && event.key.repeat == 0)
			{
				command.second.second->Execute();
			}
			break;
		}

		
	}
}

void Engine::InputManager::UpdateConsoleInput()
{
	m_XBoxController->Update();

	for (auto& command : m_ConsoleCommands)
	{
		switch (command.second.first)
		{
		case InputState::Press:
			if (m_XBoxController->IsDownThisFrame(command.first))
			{	
				command.second.second->Execute();
			}
			break;

		case InputState::Hold:
			if (m_XBoxController->IsPressed(command.first))
			{
				command.second.second->Execute();
			}
			break;

		case InputState::Release:
			if (m_XBoxController->IsUpThisFrame(command.first))
			{
				command.second.second->Execute();
			}
			break;
		}

	}
}

