#pragma once

#include "STD.hpp"
#include "FPoint.hpp"
#include "RectBase.hpp"

#include <SDL.h>

namespace ArSDL {
	class Rect : public SDL_Rect, public RectBase<Rect, int>
	{
	public:
		constexpr Rect() : SDL_Rect{} { }
		constexpr Rect(int x, int y, int w, int h) 
			: SDL_Rect{x, y, w, h} { }
		constexpr Rect(SDL_Rect const& rhs) 
			: Rect{rhs.x, rhs.y, rhs.w, rhs.h} { }
	};
}