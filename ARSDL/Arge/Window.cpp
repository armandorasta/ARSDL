#include "pch.h"
#include "Window.hpp"
#include "ArSDLError.hpp"
#include "Rect.hpp"

namespace Arge {
	Window::Window(std::string_view title, size_t width, size_t height, WindowFlags flags) :
		ptr{SDL_CreateWindow(
			title.data(), 300, 200, static_cast<int>(width),
			static_cast<int>(height), static_cast<SDL_WindowFlags>(flags)
		)}
	{
		ARSDL_ERROR_HANDLE_NULL(ptr);
	}

	Window& Window::operator=(Window&& rhs) 
	{
		if (&rhs == this)
		{
			SDL_DestroyWindow(ptr);
			ptr = std::exchange(rhs.ptr, nullptr);
		}
		return *this;
	}

	Window::~Window()
	{
		SDL_DestroyWindow(ptr);
	}

	size_t Window::GetWidth() const
	{
		int intWidth;
		SDL_GetWindowSize(ptr, &intWidth, nullptr);
		return static_cast<size_t>(intWidth);
	}

	size_t Window::GetHeight() const
	{
		int intHeight;
		SDL_GetWindowSize(ptr, nullptr, &intHeight);
		return static_cast<size_t>(intHeight);
	}

	WindowSize Window::GetSize() const
	{
		int intWidth, intHeight;
		SDL_GetWindowSize(ptr, &intWidth, &intHeight);
		return {static_cast<size_t>(intWidth), static_cast<size_t>(intHeight)};
	}

	std::string_view Window::GetTitle() const
	{
		return SDL_GetWindowTitle(ptr);
	}

	Vec2 Window::GetPosition() const
	{
		int x{};
		int y{};
		SDL_GetWindowPosition(ptr, &x, &y);
		return {static_cast<float>(x), static_cast<float>(y)};
	}

	Vec2 Window::GetCenter() const
	{
		return {GetWidth() * 0.5f, GetHeight() * 0.5f};
	}

	Rect Window::GetRect() const
	{
		return {0, 0, static_cast<int>(GetWidth()), static_cast<int>(GetHeight())};
	}

	RectF Window::GetFRect() const
	{
		return {0.0f, 0.0f, static_cast<float>(GetWidth()), static_cast<float>(GetHeight())};
	}

	bool Window::IsResizeable() const
	{
		return SDL_GetWindowFlags(ptr) & SDL_WINDOW_RESIZABLE;
	}

	bool Window::IsAlwaysOnTop() const
	{
		return SDL_GetWindowFlags(ptr) & SDL_WINDOW_ALWAYS_ON_TOP;
	}

	bool Window::IsMaximized() const
	{
		return SDL_GetWindowFlags(ptr) & SDL_WINDOW_MAXIMIZED;
	}

	bool Window::IsMinimized() const
	{
		return SDL_GetWindowFlags(ptr) & SDL_WINDOW_MINIMIZED;
	}

	bool Window::IsBorderless() const
	{
		return SDL_GetWindowFlags(ptr) & SDL_WINDOW_BORDERLESS;
	}

	FullScreenMode Window::GetFullScreen() const
	{
		if (auto const flags{SDL_GetWindowFlags(ptr)}; flags & SDL_WINDOW_FULLSCREEN)
			return FullScreenMode::Real;
		else if (flags & SDL_WINDOW_FULLSCREEN_DESKTOP)
			return FullScreenMode::Fake;
		else
			return FullScreenMode::Off;
	}

	void Window::SetSize(size_t newWidth, size_t newHeight)
	{
		ARSDL_DA(newWidth > 0);
		ARSDL_DA(newHeight > 0);
		SDL_SetWindowSize(ptr, static_cast<int>(newWidth), static_cast<int>(newHeight));
	}

	void Window::SetTitle(std::string_view newTitle)
	{
		SDL_SetWindowTitle(ptr, newTitle.data());
	}

	void Window::SetPosition(Vec2 const& newPos)
	{
		SDL_SetWindowPosition(ptr, static_cast<int>(newPos.x), static_cast<int>(newPos.y));
	}

	void Window::SetResizeable(bool toWhat) const
	{
		SDL_SetWindowResizable(ptr, toWhat ? SDL_TRUE : SDL_FALSE);
	}

	void Window::SetAlwaysOnTop(bool toWhat) const
	{
		SDL_SetWindowAlwaysOnTop(ptr, toWhat ? SDL_TRUE : SDL_FALSE);
	}

	void Window::Maximize() const
	{
		SDL_MaximizeWindow(ptr);
	}

	void Window::Minimize() const
	{
		SDL_MinimizeWindow(ptr);
	}

	void Window::SetBorderless(bool toWhat) const
	{
		SDL_SetWindowBordered(ptr, !toWhat ? SDL_TRUE : SDL_FALSE);
	}

	void Window::SetFullScreen(FullScreenMode toWhat)
	{
		ARSDL_ERROR_HANDLE_NEG(SDL_SetWindowFullscreen(ptr, static_cast<SDL_WindowFlags>(toWhat)));
	}
	
	void Window::ShowCursor(bool toggle)
	{
		SDL_ShowCursor(static_cast<bool>(toggle));
	}
	
	void Window::ShowMessageBox(MessageBoxFlag flag, std::string_view title, std::string_view message)
	{
		SDL_ShowSimpleMessageBox(static_cast<SDL_MessageBoxFlags>(flag), title.data(),
			message.data(), ptr);
	}
}
