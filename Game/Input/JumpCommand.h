#ifndef JUMP_COMMAND_H
#define JUMP_COMMAND_H

#include <Core/Input/Command.h>

namespace Engine
{
	class JumpCommand :public Command
	{
	public:
		JumpCommand(GameObject* gobj) : Command{ gobj } {};
		// Inherited via Command
		virtual void Execute(InputData data) override;
	};
}

#endif // !JUMP_COMMAND_H
