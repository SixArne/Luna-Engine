#ifndef TEXT_COMPONENT_H
#define TEXT_COMPONENT_H

#include "Component.h"

namespace Engine
{

	class TextComponent : public Component
	{
	public:
		TextComponent(std::string name) :m_Name{ name } {};

		std::string& GetText() { return m_Name; };

	private:
		std::string m_Name{};
	};
}

#endif // !TEXT_COMPONENT_H




