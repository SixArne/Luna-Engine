#ifndef SHOOT_COMMAND_H
#define SHOOT_COMMAND_H

#include <Core/Input/Command.h>

using Engine::Command;
using Engine::InputData;
using Engine::GameObject;

namespace Galaga
{
    class SpaceFighter;

	class ShootCommand final :public Command
	{
	public:
		ShootCommand(GameObject* gobj);
		// Inherited via Command
		virtual void Execute(InputData data) override;

    private:
        SpaceFighter* m_SpaceFighter{};
	};
}

#endif // !JUMP_COMMAND_H
