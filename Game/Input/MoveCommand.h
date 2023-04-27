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
		MoveCommand(GameObject* gobj, float pixelsPerSecond) : Command(gobj), m_PixelsPerSecond(pixelsPerSecond) {};
		MoveCommand(GameObject* gobj, glm::vec2 movement): Command(gobj), m_Movement(movement) {};
		MoveCommand(GameObject* gobj, glm::vec2 movement, float pixelsPerSecond): Command(gobj), m_Movement(movement), m_PixelsPerSecond(pixelsPerSecond) {};
		virtual ~MoveCommand() {};

		virtual void Execute(InputData data) override;

	private:
		glm::vec2 m_Movement;
		float m_PixelsPerSecond{50};
	};
}

#endif // !JUMP_COMMAND_H
