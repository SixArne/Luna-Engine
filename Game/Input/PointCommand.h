#ifndef POINT_COMMAND_H
#define POINT_COMMAND_H

#include <Core/Input/Command.h>

using Engine::Command;
using Engine::InputData;
using Engine::GameObject;

namespace Galaga
{
	class PointCommand final:public Command
	{
	public:
		PointCommand(GameObject* gobj) : Command{ gobj } {};
		// Inherited via Command
		virtual void Execute(InputData data) override;
	};
}

#endif // !JUMP_COMMAND_H
