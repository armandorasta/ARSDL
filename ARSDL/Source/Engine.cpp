#include "pch.h"
#include "Engine.hpp"
#include "ArSDLError.hpp"

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>

namespace ArSDL {
	Engine::Engine(std::string_view windowTitle, size_t windowWidth, size_t windowHeight)
		: m_windowCache{std::string{windowTitle}, windowWidth, windowHeight}
	{
	}

	Engine::~Engine()
	{
		SDL_Quit();
		TTF_Quit();
		IMG_Quit();
	}

	void Engine::Initialize()
	{
		ARSDL_ERROR_HANDLE_NEG(SDL_Init(SDL_INIT_VIDEO));
		ARSDL_ERROR_HANDLE_NEG(TTF_Init());

		// Initializing SDL_image
		{
			constexpr auto SDLImageInitFlags{IMG_INIT_JPG | IMG_INIT_PNG};
			ARSDL_ERROR_HANDLE_NEG(IMG_Init(SDLImageInitFlags) - SDLImageInitFlags);
		}

		auto const& [wt, ww, wh] {m_windowCache};
		m_pWindow = std::make_unique<Window>(wt, ww, wh);
		m_pRenderer = std::make_unique<Renderer>(*m_pWindow);
		
		m_optEventCache.emplace();
	}
	
	void Engine::Run()
	{
		try
		{
			Initialize();
			OnSetup();
			for (;;)
			{
				auto const frameDelta{GetFrameDelta()};
				// Display the juicy ass frame rate and delta.
				// This was a direct copy + paste from ArEngine2D xd.
				UpdateTitle(frameDelta);

				// Update mouse and keyboard (this does not interact with the event queue).
				UpdateInput();
				HandleEvents();

				OnUpdate(frameDelta);
				m_pRenderer->Present();
			}
		} 
		catch (ArSDLError const& err) 
		{ 
			std::cerr << err.GetMessage(); 
		} 
		catch (std::exception const& err) 
		{ 
			std::cerr << err.what(); 
		} 
		catch (...) 
		{ 
			std::cerr << "Unknown exception type"; 
		}
	}
	
	float Engine::GetFrameDelta()
	{
		namespace chrono = std::chrono;
		static std::pair s_TimePoints{chrono::steady_clock::now(), chrono::steady_clock::now()};

		s_TimePoints.second = chrono::steady_clock::now();
		std::chrono::duration<float> const deltaTime{s_TimePoints.second - s_TimePoints.first};
		s_TimePoints.first = s_TimePoints.second;
		return deltaTime.count();
	}
	
	void Engine::UpdateTitle(float dt)
	{
		namespace chrono = std::chrono;
		using namespace std::chrono_literals;
		constexpr chrono::duration UpdateDeltaTime{0.5s};

		static std::pair s_TimePoints{chrono::steady_clock::now(), chrono::steady_clock::now()};

		s_TimePoints.second = chrono::steady_clock::now();
		chrono::duration<float> const deltaTime{s_TimePoints.second - s_TimePoints.first};

		if (deltaTime > UpdateDeltaTime)
		{
			m_pWindow->SetTitle(std::format(
				"[{}] FPS: {}, Frame Delta: {}",
				m_windowCache.title,
				dt > 0.001f ? std::to_string(1.f / dt) : "+1000",
				dt
			));
			// m_pWindow->SetTitle(sc_InitialTitle);
			s_TimePoints.first = s_TimePoints.second;
		}
	}
	
	void Engine::HandleEvents()
	{
		SDL_Event ev{ };
		while (SDL_PollEvent(&ev)) 
		{
			if (ev.type == SDL_QUIT || ev.type == SDL_APP_TERMINATING)
				std::exit(0);
			else
				SDL_PushEvent(&ev);
		}
	}

	void Engine::UpdateInput()
	{
		m_mouse.Update();
		m_keyboard.Update();
	}

	std::optional<Event> Engine::PollNextEvent()
	{
		if (SDL_PollEvent(&m_optEventCache->GetRawAccess()))
		{
			return m_optEventCache;
		} 
		else
		{
			return { };
		}
	}

	void Engine::UnhandleLastEvent()
	{
		SDL_PushEvent(&m_optEventCache->GetRawAccess());
	}
}