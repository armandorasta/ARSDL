#include "pch.h"
#include "Font.hpp"
#include "ArSDLError.hpp"
#include "Surface.hpp"

#include <SDL/SDL_ttf.h>

namespace Arge {
	Font& Font::operator=(Font&& rhs)
	{
		if (&rhs != this)
		{
			TTF_CloseFont(ptr);
			ptr = std::exchange(rhs.ptr, nullptr);
		}
		return *this;
	}
	
	Font::~Font()
	{
		TTF_CloseFont(ptr);
	}
	
	Font::Font(std::string_view fileName, size_t pointSize) : 
		ptr{TTF_OpenFont(fileName.data(), static_cast<int>(pointSize))}
	{
		ARGE_ERROR_HANDLE_NULL(ptr);
	}

	Surface Font::RenderText(std::string_view text, Color fg)
	{
		return TTF_RenderText_Blended(ptr, text.data(), fg);
	}

	Surface Font::RenderText(std::string_view text, Color fg, Color bg)
	{
		return TTF_RenderText_Shaded(ptr, text.data(), fg, bg);
	}

	Surface Font::RenderText(std::string_view text, Color fg, uint32_t wrapLen)
	{
		return TTF_RenderText_Blended_Wrapped(ptr, text.data(), fg, wrapLen);
	}

	Surface Font::RenderText(std::string_view text, Color fg, Color bg, uint32_t wrapLen)
	{
		return TTF_RenderText_Shaded_Wrapped(ptr, text.data(), fg, bg, wrapLen);
	}

	Surface Font::RenderUTF8(std::string_view text, Color fg)
	{
		return TTF_RenderUTF8_Blended(ptr, text.data(), fg);
	}

	Surface Font::RenderUTF8(std::string_view text, Color fg, Color bg)
	{
		return TTF_RenderUTF8_Shaded(ptr, text.data(), fg, bg);
	}

	Surface Font::RenderUTF8(std::string_view text, Color fg, uint32_t wrapLen)
	{
		return TTF_RenderUTF8_Blended_Wrapped(ptr, text.data(), fg, wrapLen);
	}

	Surface Font::RenderUTF8(std::string_view text, Color fg, Color bg, uint32_t wrapLen)
	{
		return TTF_RenderUTF8_Shaded_Wrapped(ptr, text.data(), fg, bg, wrapLen);
	}

	Surface Font::RenderSingleGlyph(wchar_t glyph, Color fg)
	{
		return TTF_RenderGlyph32_Blended(ptr, glyph, fg);
	}

	Surface Font::RenderSingleGlyph(wchar_t glyph, Color fg, Color bg)
	{
		return TTF_RenderGlyph32_Shaded(ptr, glyph, fg, bg);
	}

	void Font::SetDirection(TextDirection toWhat)
	{
		TTF_SetFontDirection(ptr, static_cast<TTF_Direction>(toWhat));
	}
}
