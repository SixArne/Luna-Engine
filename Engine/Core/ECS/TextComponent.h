#ifndef TEXT_COMPONENT_H
#define TEXT_COMPONENT_H

#include "Core/ECS/Component.h"

namespace Engine
{
	class TextComponent final : public Component
	{
	public:
		TextComponent() = default;
		TextComponent(std::string text);

		virtual ~TextComponent() = default;

		TextComponent(const TextComponent& other) = delete;
		TextComponent(TextComponent&& other) = delete;
		TextComponent& operator=(const TextComponent& other) = delete;
		TextComponent& operator=(TextComponent&& other) = delete;

		virtual void ComponentUpdate() override;
		virtual void ComponentRender() override;

		std::string GetText();
		void SetText(std::string text);

	private:
		std::string m_TextToRender{};
	};
}

#endif
