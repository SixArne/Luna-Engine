#ifndef MOVE_COMMANDS_H
#define MOVE_COMMANDS_H

#include "Command.h"
#include "GameObject.h"
#include "Core/Time.h"
#include <glm/glm.hpp>

namespace Engine
{
	class ForwardCommand :public Command
	{
	public:
		ForwardCommand(GameObject* gobj) : Command{ gobj } {};
		virtual void Execute() override
		{
			float moveSpeed{ 50.f };
			glm::vec2 displacement
			{
				0,
				-moveSpeed * TIME.GetDeltaTime()
			};

			m_GameObject->GetTransform()->AddLocalPosition(displacement);
		};
	};

	class DownCommand :public Command
	{
	public:
		DownCommand(GameObject* gobj) : Command{ gobj } {};
		virtual void Execute() override
		{
			float moveSpeed{ 50.f };
			glm::vec2 displacement
			{
				0,
				moveSpeed * TIME.GetDeltaTime()
			};

			m_GameObject->GetTransform()->AddLocalPosition(displacement);
		};
	};

	class LeftCommand :public Command
	{
	public:
		LeftCommand(GameObject* gobj) : Command{ gobj } {};
		virtual void Execute() override
		{
			float moveSpeed{ 50.f };
			glm::vec2 displacement
			{
				-moveSpeed * TIME.GetDeltaTime(),
				0
			};

			m_GameObject->GetTransform()->AddLocalPosition(displacement);
		};
	};

	class RightCommand :public Command
	{
	public:
		RightCommand(GameObject* gobj) : Command{ gobj } {};
		virtual void Execute() override
		{
			float moveSpeed{ 50.f };
			glm::vec2 displacement
			{
				moveSpeed * TIME.GetDeltaTime(),
				0
			};

			m_GameObject->GetTransform()->AddLocalPosition(displacement);
		};
	};
}

#endif // !JUMP_COMMAND_H
