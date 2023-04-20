#ifndef MOVE_COMMAND_H
#define MOVE_COMMAND_H

#include <Core/Input/Command.h>
#include <GameObject.h>
#include <Core/Time.h>
#include <glm/glm.hpp>

namespace Galaga
{
	using Engine::Command;
	using Engine::GameObject;
	using Engine::InputData;

	class MoveCommand final : public Command
	{
	public:
		MoveCommand() {};
		MoveCommand(GameObject* gobj) : Command(gobj) {};
		MoveCommand(GameObject* gobj, glm::vec2 movement): Command(gobj), m_Movement(movement) {};
		virtual ~MoveCommand() {};

		virtual void Execute(InputData data) override;

	private:
		glm::vec2 m_Movement;
	};
}

#endif // !JUMP_COMMAND_H
