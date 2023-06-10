#ifndef ENGINE_RESOURCEMANAGER_H
#define ENGINE_RESOURCEMANAGER_H

#include "Singleton.h"

namespace Engine
{
	class Texture2D;
	class Font;
	class ResourceManager final : public Singleton<ResourceManager>
	{
	public:
		void Init(const std::string& data);
		std::shared_ptr<Texture2D> LoadTexture(const std::string& file) const;
		std::shared_ptr<Font> LoadFont(const std::string& file, unsigned int size) const;
		std::string LoadSound(const std::string& file) const;

	private:
		friend class Singleton<ResourceManager>;
		ResourceManager() = default;
		std::string m_dataPath;
	};
}

#endif // !ENGINE_RESOURCEMANAGER_H
