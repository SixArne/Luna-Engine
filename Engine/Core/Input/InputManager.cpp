#include "pch.h"

#include <SDL.h>
#include <variant>
#include <glm/glm.hpp>

#include "InputManager.h"
#include <backends/imgui_impl_sdl2.h>
#include <Xinput.h>
#include "Core/Input/XboxController.h"
#include "Core/Input/Command.h"

bool Engine::InputManager::ProcessInput()
{

	SDL_Event e;

	while (SDL_PollEvent(&e)) {
		if (e.type == SDL_QUIT) {
			return false;
		}

		// Process Imgui event.
		ImGui_ImplSDL2_ProcessEvent(&e);

		// Exit for Imgui viewports.
		if (e.type == SDL_WINDOWEVENT && e.window.event == SDL_WINDOWEVENT_CLOSE)
			return false;
	}

	HandleKeyboardInput();
	HandleControllerInput();

	return true;
}

Engine::InputManager::InputManager()
{

}

Engine::InputManager::~InputManager()
{

}

void Engine::InputManager::AddAction(unsigned int controllerIndex, XboxController::ControllerButton button, InputState btnState, std::unique_ptr<Command> command)
{
	auto input = ControllerInput(button, btnState);
	auto key = ControllerKey(controllerIndex, input);

	// Initialize vector of commands if no commands exist for this mapping yet.
	if (m_ControllerCommands.count(key) == 0)
	{
		m_ControllerCommands[key] = std::vector<std::unique_ptr<Command>>{};
	}

	m_ControllerCommands[key].push_back(std::move(command));
}

void Engine::InputManager::AddAction(SDL_Scancode key, InputState keyState, std::unique_ptr<Command> command)
{
	auto input = KeyboardInput(key, keyState);

	// Initialize vector of commands if no commands exist for this mapping yet.
	if (m_KeyboardCommands.count(input) == 0)
	{
		m_KeyboardCommands[input] = std::vector<std::unique_ptr<Command>>{};
	}

	m_KeyboardCommands[input].push_back(std::move(command));
}

void Engine::InputManager::AddAxisMapping(unsigned int controllerIndex, XboxController::ControllerAxis axis, std::unique_ptr<Command> command)
{
	auto input = ControllerInput(static_cast<XboxController::ControllerButton>(axis), InputState::Hold);
	auto key = ControllerKey(controllerIndex, input);

	// Initialize vector of commands if no commands exist for this mapping yet.
	if (m_ControllerCommands.count(key) == 0)
	{
		m_ControllerAxisCommands[key] = std::vector<std::unique_ptr<Command>>{};
	}

	m_ControllerAxisCommands[key].push_back(std::move(command));
}

void Engine::InputManager::AddAxisMapping(SDL_Scancode key, std::unique_ptr<Command> command)
{
	InputState state = InputState::Hold;
	auto input = KeyboardInput(key, state);

	if (m_KeyboardAxisCommands.count(input) == 0)
	{
		m_KeyboardAxisCommands[input] = std::vector<std::unique_ptr<Command>>{};
	}

	m_KeyboardAxisCommands[input].push_back(std::move(command));
}

unsigned int Engine::InputManager::AddController()
{
	auto controllerIdx = static_cast<unsigned int>(m_Controllers.size());
	m_Controllers.push_back(std::make_unique<XboxController>(controllerIdx));
	return controllerIdx;
}

void Engine::InputManager::HandleControllerInput()
{
	for (auto& controller : m_Controllers)
	{
		controller->Update();
	}

	// Loop over every input, commands mapping
	for (auto& controllerMapping : m_ControllerCommands)
	{
		const auto inputState = controllerMapping.first.second.second;
		const auto controllerIndex = controllerMapping.first.first;
		const auto button = controllerMapping.first.second.first;
		const auto& commands = controllerMapping.second;

		switch (inputState)
		{
		case InputState::Press:
			if (m_Controllers[controllerIndex]->IsPressedThisFrame(button))
			{
				InputData data = true;
				for (auto& command : commands) { command->Execute(data); }
			}
			break;
		case InputState::Release:
			if (m_Controllers[controllerIndex]->IsReleasedThisFrame(button))
			{
				InputData data = true;
				for (auto& command : commands) { command->Execute(data); }
			}
			break;
		case InputState::Hold:
			if (m_Controllers[controllerIndex]->IsPressed(button))
			{
				InputData data = true;
				for (auto& command : commands) { command->Execute(data); }
			}
			break;
		default:
			break;
		}
	}

	for (auto& controllerMapping : m_ControllerAxisCommands)
	{
		const auto inputState = static_cast<XboxController::ControllerAxis>(controllerMapping.first.second.first);
		const auto controllerIndex = controllerMapping.first.first;
		const auto& commands = controllerMapping.second;

		switch (inputState)
		{
			case XboxController::ControllerAxis::LeftThumbX:
			{
				InputData data{ m_Controllers[controllerIndex]->GetLeftThumbStick().x };
				for (auto& command : commands) { command->Execute(data); }
				break;
			}
			case XboxController::ControllerAxis::LeftThumbY:
			{
				InputData data{ m_Controllers[controllerIndex]->GetLeftThumbStick().y };
				for (auto& command : commands) { command->Execute(data); }
				break;
			}
			case XboxController::ControllerAxis::LeftThumb:
			{
				InputData data{ m_Controllers[controllerIndex]->GetLeftThumbStick() };
				for (auto& command : commands) { command->Execute(data); }
				break;
			}
			case XboxController::ControllerAxis::RightThumbX:
			{
				InputData data{ m_Controllers[controllerIndex]->GetRightThumbStick().x };
				for (auto& command : commands) { command->Execute(data); }
				break;
			}
			case XboxController::ControllerAxis::RightThumbY:
			{
				InputData data{ m_Controllers[controllerIndex]->GetRightThumbStick().y };
				for (auto& command : commands) { command->Execute(data); }
				break;
			}
			case XboxController::ControllerAxis::RightThumb:
			{
				InputData data{ m_Controllers[controllerIndex]->GetRightThumbStick() };
				for (auto& command : commands) { command->Execute(data); }
				break;
			}
			case XboxController::ControllerAxis::LeftTrigger:
			{
				InputData data{ m_Controllers[controllerIndex]->GetLeftTrigger() };
				for (auto& command : commands) { command->Execute(data); }
				break;
			}
			case XboxController::ControllerAxis::RightTrigger:
			{
				InputData data{ m_Controllers[controllerIndex]->GetRightTrigger() };
				for (auto& command : commands) { command->Execute(data); }
				break;
			}
			default:
				break;
		}
	}
}

void Engine::InputManager::HandleKeyboardInput()
{
	// Update the keyboard
	m_Keyboard->Update();

	for (auto& mapping : m_KeyboardCommands)
	{
		const auto inputState = mapping.first.second;
		const auto button = mapping.first.first;
		const auto& commands = mapping.second;

		// Check press or release
		switch (inputState)
		{
		case InputState::Press:
			if (m_Keyboard->IsPressedThisFrame(button))
			{
				// Execute every command in the mapping (vector of commands)
				InputData data = true;
				for (auto& command : commands) { command->Execute(data); }
			}
			break;
		case InputState::Release:
			if (m_Keyboard->IsReleasedThisFrame(button))
			{
				// Execute every command in the mapping (vector of commands)
				InputData data = true;
				for (auto& command : commands) { command->Execute(data); }
			}
			break;
		case InputState::Hold:
			if (m_Keyboard->IsPressed(button))
			{
				// Execute every command in the mapping (vector of commands)
				InputData data = true;
				for (auto& command : commands) { command->Execute(data); }
			}
			break;

		default:
			L_DEBUG("Unknown InputState! Shouldn't happen!");
			break;
		}
	}
	for (auto& mapping : m_KeyboardAxisCommands)
	{
		const auto button = mapping.first.first;
		const auto& commands = mapping.second;

		if (m_Keyboard->IsPressed(button))
		{
			// Execute every command in the mapping (vector of commands)
			for (auto& command : commands)
			{
				InputData data = true;
				command->Execute(data);
			}
		}
	}
}

bool Engine::InputManager::HasController(unsigned int controllerIdx)
{
	return controllerIdx < m_Controllers.size();
}