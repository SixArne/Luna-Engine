#ifndef TEXTURE_RENDERER_H
#define TEXTURE_RENDERER_H

#include <string>
#include <memory>

#include <SDL.h>
#include <glm/vec2.hpp>

#include "Core/ECS/Component.h"



namespace Engine
{
	class Texture2D;

	class TextureRenderer final: public Component
	{
	public:
		TextureRenderer() = default;
		TextureRenderer(GameObject* object);
		TextureRenderer(GameObject* object, const std::string& textureFile);
		TextureRenderer(GameObject* object, std::shared_ptr<Texture2D> texture);

		~TextureRenderer();

		TextureRenderer(const TextureRenderer& other) = delete;
		TextureRenderer(TextureRenderer&& other) = delete;
		TextureRenderer& operator=(const TextureRenderer& other) = delete;
		TextureRenderer& operator=(TextureRenderer&& other) = delete;

		virtual void Update() override;
		virtual void Render() override;
		virtual void Init() override;

		void SetTexture(std::string source);
		void SetTexture(std::shared_ptr<Texture2D> texture);

		void SetEmptyTexture();

		glm::ivec2 GetScreenSize();

		void OnImGui() override;

	private:
		std::shared_ptr<Texture2D> m_Texture{};
	};
}

#endif
