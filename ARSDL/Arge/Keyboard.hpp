#pragma once
#include "ArgeCore.hpp"
#include "KeyState.hpp"
#include "Enums.hpp"

#include <SDL/SDL.h>

namespace Arge {
	class Keyboard
	{
	public:
		Keyboard();
		Keyboard(Keyboard const&)            = delete;
		Keyboard(Keyboard&&)                 = delete;
		Keyboard& operator=(Keyboard const&) = delete;
		Keyboard& operator=(Keyboard&&)      = delete;

		void Flush();
		void Update();

		constexpr KeyState const& operator[](Keycode kc) const
		{
			return m_keys[SDL_GetScancodeFromKey(static_cast<SDL_Keycode>(kc))];
		}

		KeyState const& operator[](char ch) const;

		constexpr size_t GetKeyCount() const
		{
			return m_keys.size();
		}

	private:
		std::vector<KeyState> m_keys;
	};
}