#pragma once
#include "STD.hpp"
#include "KeyState.hpp"
#include "Enums.hpp"

#include <SDL.h>

namespace ArSDL {
	class Keyboard
	{
	public:
		Keyboard();
		Keyboard(Keyboard const&)            = delete;
		Keyboard(Keyboard&&)                 = delete;
		Keyboard& operator=(Keyboard const&) = delete;
		Keyboard& operator=(Keyboard&&)      = delete;

	public:
		void Flush();
		void Update();

		constexpr KeyState const& operator[](Keycode kc) const
		{
			return m_keys[SDL_GetScancodeFromKey(static_cast<SDL_Keycode>(kc))];
		}

		constexpr size_t GetKeyCount() const
		{
			return m_keys.size();
		}

	private:
		std::vector<KeyState> m_keys;
	};
}