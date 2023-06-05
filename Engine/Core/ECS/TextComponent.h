#ifndef TEXT_COMPONENT_H
#define TEXT_COMPONENT_H

#include "Core/ECS/Component.h"
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

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
		TextComponent(GameObject* object, std::string text, glm::vec2 offset);

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
		void SetColor(const glm::ivec3& color);
		void SetOffset(glm::vec2 offset);
		glm::ivec2 GetScreenSize();

	private:
		void RenderText();

		std::string m_TextToRender{"empty"};
		std::shared_ptr<Font> m_Font{};
		std::shared_ptr<Texture2D> m_TextTexture{};

		glm::vec2 m_Offset{};
		glm::ivec3 m_Color{255, 255, 255};

		bool m_MarkedForUpdate{ true };
	};
}

#endif
