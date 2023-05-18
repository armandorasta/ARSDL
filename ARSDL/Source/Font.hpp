#pragma once
#include "STD.hpp"
#include "Color.hpp"
#include "Enums.hpp"

#include <SDL.h>
#include <SDL_ttf.h>

namespace ArSDL {
	class Surface;

	class Font
	{
	public:
		Font(Font const&)            = default;
		Font& operator=(Font const&) = default;
		Font& operator=(Font&&);
		~Font();

		constexpr Font(Font&& rhs) noexcept : ptr{std::exchange(rhs.ptr, nullptr)}
		{
		}

		constexpr Font(TTF_Font* myPtr) noexcept : ptr{myPtr}
		{
		}

		Font(std::string_view fileName, size_t pointSize);

	public:
		Surface RenderText(std::string_view text, Color fg);
		Surface RenderText(std::string_view text, Color fg, Color bg);
		Surface RenderText(std::string_view text, Color fg, uint32_t wrapLen);
		Surface RenderText(std::string_view text, Color fg, Color bg, uint32_t wrapLen);

		Surface RenderUTF8(std::string_view text, Color fg);
		Surface RenderUTF8(std::string_view text, Color fg, Color bg);
		Surface RenderUTF8(std::string_view text, Color fg, uint32_t wrapLen);
		Surface RenderUTF8(std::string_view text, Color fg, Color bg, uint32_t wrapLen);

		Surface RenderSingleGlyph(wchar_t glyph, Color fg);
		Surface RenderSingleGlyph(wchar_t glyph, Color fg, Color bg);

		void SetDirection(TextDirection toWhat);

	public:
		TTF_Font* ptr;
	};
}