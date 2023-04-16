#ifndef COMMAND_H
#define COMMAND_H

#include <variant>
#include <glm/glm.hpp>

#include "InputManager.h"
#include "GameObject.h"

namespace Engine
{

	class Command
	{
	public:
		Command(class GameObject* gobj) : m_GameObject{ gobj } {};
		virtual ~Command() {};
		virtual void Execute(InputData data) = 0;
		virtual void Undo() {};

	protected:
		GameObject* m_GameObject{};
	};
}

#endif // !COMMAND_H
