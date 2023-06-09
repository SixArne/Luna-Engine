#ifndef SWITCH_SCENE_COMMAND_H
#define SWITCH_SCENE_COMMAND_H

#include <Core/Input/Command.h>

using Engine::Command;
using Engine::InputData;
using Engine::GameObject;

namespace Galaga
{
	class SwitchSceneCommand :public Command
	{
	public:
		SwitchSceneCommand(GameObject* gobj) : Command{ gobj } {};
		virtual ~SwitchSceneCommand() = default;
		// Inherited via Command
		virtual void Execute(InputData data) override;
	};
}

#endif // !JUMP_COMMAND_H
