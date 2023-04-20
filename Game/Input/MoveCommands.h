#ifndef MOVE_COMMANDS_H
#define MOVE_COMMANDS_H

#include <Core/Input/Command.h>
#include <GameObject.h>
#include <Core/Time.h>
#include <Core/Log.h>
#include <glm/glm.hpp>

namespace Engine
{
	class MoveCommand final : public Command
	{
	public:
		MoveCommand(GameObject* gobj) : Command(gobj) {};
		virtual ~MoveCommand() {};

		virtual void Execute(InputData data) override
		{
			glm::vec2 offset = std::get<glm::vec2>(data);
			m_GameObject->GetTransform()->AddLocalPosition(offset * TIME.GetDeltaTime());
		};
	};
}

#endif // !JUMP_COMMAND_H
