#ifndef TEXTURE_RENDERER_H
#define TEXTURE_RENDERER_H

#include <SDL.h>
#include <glm/vec2.hpp>

#include "Core/ECS/Component.h"



namespace Engine
{
	class Texture2D;

	class TextureRendererComponent final: public Component
	{
	public:
		TextureRendererComponent() = default;
		TextureRendererComponent(GameObject* object);
		TextureRendererComponent(GameObject* object, const std::string& textureFile);

		~TextureRendererComponent();

		TextureRendererComponent(const TextureRendererComponent& other) = delete;
		TextureRendererComponent(TextureRendererComponent&& other) = delete;
		TextureRendererComponent& operator=(const TextureRendererComponent& other) = delete;
		TextureRendererComponent& operator=(TextureRendererComponent&& other) = delete;

		virtual void Update() override;
		virtual void Render() override;
		virtual void Init() override;

		void SetTexture(std::string source);

	private:
		std::shared_ptr<Texture2D> m_Texture{};
	};
}

#endif
