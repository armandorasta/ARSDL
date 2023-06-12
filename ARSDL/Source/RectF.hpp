#pragma once

#include "STD.hpp"
#include "RectBase.hpp"

#include <SDL.h>

namespace Arge {
	class RectF : public SDL_FRect, public RectBase<RectF, float>
	{
	public:
		constexpr RectF() : SDL_FRect{} { }
		constexpr RectF(float x, float y, float w, float h) : SDL_FRect{x, y, w, h}
		{
		}
	};
}