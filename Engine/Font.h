#ifndef ENGINE_FONT_H
#define ENGINE_FONT_H

#include <string>

struct _TTF_Font;
namespace Engine
{
	/**
	 * Simple RAII wrapper for a _TTF_Font
	 */
	class Font final
	{
	public:
		_TTF_Font* GetFont() const;
		explicit Font(const std::string& fullPath, unsigned int size);
		~Font();

		Font(const Font &) = delete;
		Font(Font &&) = delete;
		Font & operator= (const Font &) = delete;
		Font & operator= (const Font &&) = delete;
	private:
		_TTF_Font* m_font;
		unsigned int m_size;
	};
}

#endif // !ENGINE_FONT_H
