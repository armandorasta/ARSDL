#pragma once
#include "STD.hpp"

#include "Enums.hpp"
#include "Window.hpp"
#include "Renderer.hpp"
#include "Event.hpp"
#include "Keyboard.hpp"
#include "Mouse.hpp"

namespace Arge {
	class Engine
	{
	private:
		struct WindowInfoCache
		{
			std::string title;
			size_t width;
			size_t height;
		};

	public:
		Engine(std::string_view windowTitle, size_t windowWidth, size_t windowHeight);
		virtual ~Engine();


		virtual void OnSetup() = 0;
		virtual void OnUpdate(float dt) = 0;


		void Initialize();
		void Run();


		std::optional<Event> PollNextEvent();
		void UnhandleLastEvent();

	private:
		float GetFrameDelta();
		void UpdateTitle(float dt);
		void HandleEvents();
		void UpdateInput();

	public:
		constexpr Window& GetWindow()
		{
			return *m_pWindow;
		}

		constexpr Renderer& GetRenderer()
		{
			return *m_pRenderer;
		}


		/* I just like this syntax better than the getters */

 		Keyboard const& keyboard{m_keyboard};
		Mouse const& mouse{m_mouse};

	private:
		// Stores the window info in constructor for later use (used only once tho).
		WindowInfoCache m_windowCache;

		// Mouse and keyboard have to be private, so the user is unable to mutate them.
		Mouse m_mouse{};
		Keyboard m_keyboard{};

		// These need to be unique ptrs because the constructors need to run inside 
		// a try-catch block; They could also be optionals but who cares?
		std::unique_ptr<Window> m_pWindow;
		std::unique_ptr<Renderer> m_pRenderer;

		std::optional<Event> m_optEventCache{};
	};
}