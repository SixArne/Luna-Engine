#pragma once
#include "Singleton.h"
#include <Xinput.h>
#include <SDL.h>
#include <glm/glm.hpp>
#include "Core/Input/Command.h"
#include "Core/Input/XboxController.h"

namespace Engine
{
	template<class T>
	concept IsCommand = std::is_base_of<T, Command>::value;

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

		XboxController* GetController() const;

		template<typename T>
		void CreateConsoleCommand(int key, InputState state, class GameObject* component);

		template<typename T>
		void CreateDesktopCommand(int key, InputState state, class GameObject* component);

		void CreateController(int controllerIndex);

	private:		

		void UpdateComputerInput(SDL_Event event);
		void UpdateConsoleInput();

		using CommandsMap = std::map<int, std::pair<InputState, std::unique_ptr<Command>>>;
		
		CommandsMap m_ConsoleCommands{};
		CommandsMap m_DesktopCommands{};

		std::unique_ptr<XboxController> m_XBoxController{};
	};

	template<typename IsCommand>
	void InputManager::CreateConsoleCommand(int key, InputState state, GameObject* gameobject)
	{
		std::unique_ptr<IsCommand> command = std::make_unique<IsCommand>(gameobject);
		m_ConsoleCommands.insert(std::pair{ key, std::pair{state, std::move(command)} });
	}

	template<typename IsCommand>
	void InputManager::CreateDesktopCommand(int key, InputState state, GameObject* gameobject)
	{
		std::unique_ptr<IsCommand> command = std::make_unique<IsCommand>(gameobject);
		m_DesktopCommands.insert(std::pair{ key, std::pair{state, std::move(command)} });
	}
}
