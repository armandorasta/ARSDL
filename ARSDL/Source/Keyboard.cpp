#include "pch.h"
#include "Keyboard.hpp"

namespace ArSDL {
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
}
