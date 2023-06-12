#pragma once
#include "STD.hpp"
#include "KeyState.hpp"
#include "Vec2.hpp"

#include <SDL.h>

namespace Arge {
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
		Vec2 pos;
		KeyState left;
		KeyState mid;
		KeyState right;
	};
}