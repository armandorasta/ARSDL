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
		auto const mouse{GetMouseState()};

		static sdl::Color clearColor{sdl::Colors::Cyan};
		while (auto optEv{PollNextEvent()})
		{
			auto& ev{*optEv};
			switch (static_cast<sdl::EventType>(ev.type))
			{
			case sdl::EventType::KeyDown:
				if (static_cast<sdl::KeyCode>(ev.key.keysym.sym) == sdl::KeyCode::Space)
				{
					clearColor = clearColor == sdl::Colors::Cyan
						? sdl::Colors::Green
						: sdl::Colors::Cyan;
				}
				break;
			default:
				IgnoreLastEvent();
				break;
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