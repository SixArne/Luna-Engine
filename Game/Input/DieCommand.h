#ifndef DIE_COMMAND_H
#define DIE_COMMAND_H

#include <Core/Input/Command.h>

using Engine::Command;
using Engine::InputData;
using Engine::GameObject;

namespace Galaga
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
