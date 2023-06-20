#include "pch.h"
#include "Keyboard.hpp"

namespace Arge {
	Keyboard::Keyboard()
	{
		int keyCount;
		(void)SDL_GetKeyboardState(&keyCount);
		m_keys.resize(static_cast<size_t>(keyCount));
	}
	
	void Keyboard::Flush()
	{
		std::memset(m_keys.data(), 0, m_keys.size() * sizeof KeyState);
	}
	
	void Keyboard::Update()
	{
		auto const states{SDL_GetKeyboardState(nullptr)};
		for (size_t i{ }; i < m_keys.size(); ++i)
		{
			m_keys[i].Update(states[i]);
		}
	}

	KeyState const& Keyboard::operator[](char ch) const
	{
		auto const myChar{std::tolower(ch)};
		ARSDL_DA('a' <= myChar && myChar <= 'z');
		return m_keys[SDL_GetScancodeFromKey(static_cast<SDL_Keycode>(ch))];
	}
}
