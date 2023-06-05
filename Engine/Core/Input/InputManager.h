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
#include <unordered_map>

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

	using ControllerInput = std::pair<XboxController::ControllerButton, InputState>;
	using ControllerKey = std::pair<unsigned int, ControllerInput>;
	using ControllerCommandsMap = std::map<ControllerKey, std::vector<std::unique_ptr<Command>>>;
	using ControllerAxisCommandMap = std::map<ControllerKey, std::vector<std::unique_ptr<Command>>>;

	using KeyboardInput = std::pair<SDL_Scancode, InputState>;
	using KeyboardCommandsMap = std::map<KeyboardInput, std::vector<std::unique_ptr<Command>>>;
	using KeyboardAxisCommandsMap = std::map<KeyboardInput, std::vector<std::unique_ptr<Command>>>;

	class Schema
	{
	public:
		Schema() = default;
		~Schema() = default;

		void AddAction(unsigned int controllerIndex, XboxController::ControllerButton button, InputState btnState, std::unique_ptr<Command> command);
		void AddAction(SDL_Scancode key, InputState keyState, std::unique_ptr<Command> command);

		void AddAxisMapping(unsigned int controllerIndex, XboxController::ControllerAxis axis, std::unique_ptr<Command> command);
		void AddAxisMapping(SDL_Scancode key, std::unique_ptr<Command> command);

		void SetActive(bool value) {m_IsActive = value; }
		bool IsActive() { return m_IsActive; }
		void Clear();

	private:
		friend class InputManager;

		ControllerCommandsMap m_ControllerCommands{};
		ControllerAxisCommandMap m_ControllerAxisCommands{};
		KeyboardCommandsMap m_KeyboardCommands{};
		KeyboardAxisCommandsMap m_KeyboardAxisCommands{};

		bool m_IsActive{true};
	};

	class InputManager final : public Singleton<InputManager>
	{
	public:
		bool ProcessInput();

		unsigned int AddController();
		bool HasController(unsigned int controllerIdx);

		Schema* GetSchema(const std::string& name);
		Schema* AddSchema(const std::string& name);

	private:
		friend class Singleton<InputManager>;
		InputManager();

		virtual ~InputManager();
		void HandleControllerInput();
		void HandleKeyboardInput();

		std::unordered_map<std::string, std::unique_ptr<Schema>> m_Schemas{};

		std::vector<std::unique_ptr<XboxController>> m_Controllers{};
		std::unique_ptr<Keyboard> m_Keyboard{ std::make_unique<Keyboard>() };
	};
}


#endif