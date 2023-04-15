#ifndef POINT_COMMAND_H
#define POINT_COMMAND_H

#include "Command.h"

namespace Engine
{
	class PointCommand :public Command
	{
	public:
		PointCommand(GameObject* gobj) : Command{ gobj } {};
		// Inherited via Command
		virtual void Execute() override;
	};
}

#endif // !JUMP_COMMAND_H
