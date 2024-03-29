#ifndef ENGINE_RENDERER_H
#define ENGINE_RENDERER_H

#include <SDL.h>
#include "Singleton.h"

namespace Engine
{
	class Texture2D;
	class SceneManager;
	/**
	 * Simple RAII wrapper for the SDL renderer
	 */
	class Renderer final : public Singleton<Renderer>
	{
		SDL_Renderer* m_renderer{};
		SDL_Window* m_window{};
		SDL_Color m_clearColor{};
	public:
		void Init(SDL_Window* window);
		void Render() const;
		void RenderImGui(SceneManager& sceneManager);
		void Destroy();

		void RenderTexture(const Texture2D& texture, float x, float y) const;
		void RenderTexture(const Texture2D& texture, float x, float y, float width, float height) const;

		inline SDL_Renderer* GetSDLRenderer() const { return m_renderer; };

		const SDL_Color& GetBackgroundColor() const { return m_clearColor; }
		void SetBackgroundColor(const SDL_Color& color) { m_clearColor = color; }
	};
}

#endif // !ENGINE_RENDERER_H
