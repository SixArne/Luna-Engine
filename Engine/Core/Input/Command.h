#ifndef COMMAND_H
#define COMMAND_H

namespace Engine
{
	class Command
	{
	public:
		Command(class GameObject* gobj) : m_GameObject{ gobj } {};
		virtual ~Command() {};
		virtual void Execute() = 0;
		virtual void Undo() {};

	protected:
		GameObject* m_GameObject{};
	};
}

#endif // !COMMAND_H
