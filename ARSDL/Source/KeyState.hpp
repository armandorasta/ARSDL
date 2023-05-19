#pragma once
#include "STD.hpp"

namespace ArSDL {
	class KeyState
	{
	public:
		constexpr KeyState()                           = default;
		constexpr KeyState(KeyState const&)            = default;
		constexpr KeyState(KeyState&&)                 = default;
		constexpr KeyState& operator=(KeyState const&) = default;
		constexpr KeyState& operator=(KeyState&&)      = default;

	public:
		constexpr void Update(bool bStateCurrFrame)
		{
			bPressed = !bDown && bStateCurrFrame;
			bReleased = bDown && !bStateCurrFrame;
			bDown = bStateCurrFrame;
		}

		constexpr operator bool() const
		{
			return bDown;
		}

		constexpr bool IsDown() const
		{
			return bDown;
		}

		constexpr bool IsPressed() const
		{
			return bPressed;
		}

		constexpr bool IsJustReleased() const
		{
			return bReleased;
		}

	private:
		// true whenever the button is clicked and as long as it remains clicks.
		bool bDown;
		// true only in the frame the button is hit.
		bool bPressed;
		// true only in the frame the button is released.
		bool bReleased;
	};
}