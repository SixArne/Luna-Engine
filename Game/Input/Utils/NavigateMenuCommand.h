#ifndef NAVIGATE_MENU_COMMAND_H
#define NAVIGATE_MENU_COMMAND_H

#include <Core/Input/Command.h>

using Engine::Command;
using Engine::InputData;
using Engine::GameObject;

namespace Galaga
{
    class OptionContainer;

    enum class NavigateMenuCommandType
    {
        Next,
        Previous,
        Click
    };

	class NavigateMenuCommand final: public Command
	{
	public:
		NavigateMenuCommand(GameObject* gobj, NavigateMenuCommandType type);
		// Inherited via Command
		virtual void Execute(InputData data) override;

    private:
        OptionContainer* m_OptionContainer{};
        NavigateMenuCommandType m_Type{};
    };
}

#endif // !JUMP_COMMAND_H
