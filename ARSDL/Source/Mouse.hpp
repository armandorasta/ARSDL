#pragma once
#include "STD.hpp"
#include "KeyState.hpp"
#include "FPoint.hpp"

#include <SDL.h>

namespace ArSDL {
	class Mouse
	{
	public:
		Mouse()                        = default;
		Mouse(Mouse const&)            = delete;
		Mouse(Mouse&&)                 = delete;
		Mouse& operator=(Mouse const&) = delete;
		Mouse& operator=(Mouse&&)      = delete;

	public:
		void Update();

	public:
		FPoint pos;
		KeyState left;
		KeyState mid;
		KeyState right;
	};
}