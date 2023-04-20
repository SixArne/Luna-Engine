#ifndef DIE_COMMAND_H
#define DIE_COMMAND_H

#include <Core/Input/Command.h>

namespace Engine
{
	class DieCommand :public Command
	{
	public:
		DieCommand(GameObject* gobj) : Command{ gobj } {};
		// Inherited via Command
		virtual void Execute(InputData data) override;
	};
}

#endif // !JUMP_COMMAND_H
