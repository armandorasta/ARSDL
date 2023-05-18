#pragma once

#include "STD.hpp"
#include "RectBase.hpp"

#include <SDL.h>

namespace ArSDL {
	class FRect : public SDL_FRect, public RectBase<FRect, float>
	{
	public:
		constexpr FRect() : SDL_FRect{} { }
		constexpr FRect(float x, float y, float w, float h) : SDL_FRect{x, y, w, h}
		{
		}
	};
}