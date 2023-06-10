#include <string>
#include <memory>

#include <SDL_image.h>
#include <SDL_ttf.h>
#include "ResourceManager.h"
#include "Renderer.h"
#include "Texture2D.h"
#include "Font.h"
#include "Core/Log.h"

void Engine::ResourceManager::Init(const std::string& dataPath)
{
	m_dataPath = dataPath;

	if (TTF_Init() != 0)
	{
		throw std::runtime_error(std::string("Failed to load support for fonts: ") + SDL_GetError());
	}
}

std::string Engine::ResourceManager::LoadSound(const std::string& file) const
{
	const auto fullPath = m_dataPath + file;
	return fullPath;
}

std::shared_ptr<Engine::Texture2D> Engine::ResourceManager::LoadTexture(const std::string& file) const
{
	const auto fullPath = m_dataPath + file;
	auto texture = IMG_LoadTexture(Renderer::GetInstance().GetSDLRenderer(), fullPath.c_str());
	if (texture == nullptr)
	{
		L_ERROR("Failed to load texture");
		__debugbreak();
	}
	return std::make_shared<Texture2D>(texture);
}

std::shared_ptr<Engine::Font> Engine::ResourceManager::LoadFont(const std::string& file, unsigned int size) const
{
	return std::make_shared<Font>(m_dataPath + file, size);
}
