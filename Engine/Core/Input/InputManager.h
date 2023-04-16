#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

#include <variant>
#include "Singleton.h"
#include <Xinput.h>
#include <SDL.h>
#include <memory>
#include <glm/glm.hpp>
#include "Core/Input/XboxController.h"
#include "Core/Input/Keyboard.h"

namespace Engine
{
	using InputData = std::variant<bool, float, glm::vec2>;

	class Command;

	enum class InputState
	{
		Hold,
		Press,
		Release
	};

	class InputManager final : public Singleton<InputManager>
	{
	public:
		bool ProcessInput();

		void AddAction(unsigned int controllerIndex, XboxController::ControllerButton button, InputState btnState, std::unique_ptr<Command> command);
		void AddAction(SDL_Scancode key, InputState keyState, std::unique_ptr<Command> command);

		void AddAxisMapping(unsigned int controllerIndex, XboxController::ControllerAxis axis, std::unique_ptr<Command> command);
		void AddAxisMapping(SDL_Scancode key, std::unique_ptr<Command> command);

		unsigned int AddController();

	private:
		friend class Singleton<InputManager>;
		InputManager();

		virtual ~InputManager();
		void HandleControllerInput();
		void HandleKeyboardInput();

		using ControllerInput = std::pair<XboxController::ControllerButton, InputState>;
		using ControllerKey = std::pair<unsigned int, ControllerInput>;
		using ControllerCommandsMap = std::map<ControllerKey, std::vector<std::unique_ptr<Command>>>;
		using ControllerAxisCommandMap = std::map<ControllerKey, std::vector<std::unique_ptr<Command>>>;

		using KeyboardInput = std::pair<SDL_Scancode, InputState>;
		using KeyboardCommandsMap = std::map<KeyboardInput, std::vector<std::unique_ptr<Command>>>;
		using KeyboardAxisCommandsMap = std::map<KeyboardInput, std::vector<std::unique_ptr<Command>>>;

		ControllerCommandsMap m_ControllerCommands{};
		ControllerAxisCommandMap m_ControllerAxisCommands{};
		KeyboardCommandsMap m_KeyboardCommands{};
		KeyboardAxisCommandsMap m_KeyboardAxisCommands{};

		std::vector<std::unique_ptr<XboxController>> m_Controllers{};
		std::unique_ptr<Keyboard> m_Keyboard{ std::make_unique<Keyboard>() };
	};
}


#endif