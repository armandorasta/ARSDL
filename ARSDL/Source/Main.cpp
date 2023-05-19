#include "pch.h"
#include <iostream>
#include <chrono>
#include <thread>

#include <SDL.h>
#include <glad/glad.h>
#include <SDL_ttf.h>
#include <SDL_image.h>

#include "Window.hpp"
#include "Renderer.hpp"
#include "ArSDLError.hpp"
#include "Font.hpp"
#include "Engine.hpp"

namespace sdl = ArSDL;

class MyGame : public sdl::Engine 
{
public:
	using Engine::Engine;

	void OnSetup() override 
	{
	}

	void OnUpdate(float) override 
	{
		auto& gfx{GetRenderer()};

		static sdl::Color clearColor{};

		// if (mouse.left.IsJustReleased())
		// {
		// 	clearColor = clearColor == sdl::Colors::Orange 
		// 		? sdl::Colors::Cyan 
		// 		: sdl::Colors::Orange;
		// }

		while (auto optEv{PollNextEvent()})
		{
			auto& ev{*optEv};
			switch (ev.GetType())
			{
			case sdl::EventType::KeyDown:
			{
				auto const keyEv{ev.AsKeyboardEvent()};
				if (keyEv == sdl::Keycode::Space)
				{
					clearColor = clearColor == sdl::Colors::Cyan
						? sdl::Colors::Green
						: sdl::Colors::Cyan;
				}
				break;
			}
			}
		}

		constexpr auto W{150.0f};

		gfx.Clear(clearColor);
		gfx.DrawRect(mouse.pos, W, W, sdl::Colors::Aqua, 5.0f);
	}
};

int main(int, char**)
{
	MyGame game{"Tiddies", 1080, 720};
	game.Run();
	return 0;
}