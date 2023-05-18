#pragma once
#include "STD.hpp"

#include "Enums.hpp"
#include "Window.hpp"
#include "Renderer.hpp"
#include "Event.hpp"

namespace ArSDL {
	class Engine
	{
	private:
		struct WindowInfoCache
		{
			std::string title;
			size_t width;
			size_t height;
		};

		struct MouseState
		{
			FPoint pos;
			bool bLeft;
			bool bMid;
			bool bRight;
		};

	public:
		Engine(std::string_view windowTitle, size_t windowWidth, size_t windowHeight);
		~Engine();

	public:
		void Initialize();
		void Run();

		virtual void OnSetup() = 0;
		virtual void OnUpdate(float dt) = 0;

		MouseState GetMouseState() const;
		std::optional<Event> PollNextEvent()
		{
			if (SDL_PollEvent(&m_optEventCache->GetRawAccess()))
			{
				return m_optEventCache;
			}
			else
			{
				return {};
			}
		}

		// Should be placed in the default label of the event switch statement.
		void IgnoreLastEvent()
		{
			SDL_PushEvent(&m_optEventCache->GetRawAccess());
		}

	private:
		float GetFrameDelta();
		void UpdateTitle(float dt);
		void HandleEvents();

	public:
		constexpr Window& GetWindow()
		{
			return *m_pWindow;
		}

		constexpr Renderer& GetRenderer()
		{
			return *m_pRenderer;
		}

	private:
		// Stores the window info in constructor for later use.
		WindowInfoCache m_windowCache;

		// These need to be unique ptrs because the constructors need to run inside 
		// a try-catch block.
		std::unique_ptr<Window> m_pWindow;
		std::unique_ptr<Renderer> m_pRenderer;

		std::optional<Event> m_optEventCache{};
	};
}