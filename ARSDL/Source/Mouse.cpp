#include "pch.h"
#include "Mouse.hpp"

namespace ArSDL {
	void Mouse::Update()
	{
		int x, y;
		auto const buttFlags{SDL_GetMouseState(&x, &y)};
		pos = {static_cast<float>(x), static_cast<float>(y)};
		left.Update((buttFlags & SDL_BUTTON(SDL_BUTTON_LEFT)) != 0);
		mid.Update((buttFlags & SDL_BUTTON(SDL_BUTTON_MIDDLE)) != 0);
		right.Update((buttFlags & SDL_BUTTON(SDL_BUTTON_RIGHT)) != 0);
	}
}