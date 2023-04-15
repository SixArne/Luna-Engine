#ifndef TEXT_COMPONENT_H
#define TEXT_COMPONENT_H

#include "Core/ECS/Component.h"

namespace Engine
{
	class Font;
	class Texture2D;

	class TextComponent: public Component
	{
	public:
		TextComponent(GameObject* object);
		TextComponent(GameObject* object, std::string text, std::shared_ptr<Font> font);
		TextComponent(GameObject* object, std::string text);

		virtual ~TextComponent() = default;

		TextComponent(const TextComponent& other) = delete;
		TextComponent(TextComponent&& other) = delete;
		TextComponent& operator=(const TextComponent& other) = delete;
		TextComponent& operator=(TextComponent&& other) = delete;

		virtual void Init() override;
		virtual void Update() override;
		virtual void Render() override;

		std::string GetText();
		void SetText(std::string text);
		void SetFont(std::shared_ptr<Font> font);

	private:
		std::string m_TextToRender{"empty"};
		std::shared_ptr<Font> m_Font{};
		std::shared_ptr<Texture2D> m_TextTexture{};

		bool m_MarkedForUpdate{ true };
	};
}

#endif
