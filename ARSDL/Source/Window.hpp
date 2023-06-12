#pragma once
#include <SDL.h>

#include "STD.hpp"
#include "Vec2.hpp"
#include "Enums.hpp"
#include "RectF.hpp"

namespace Arge {
	class Window
	{
	public:
		Window(Window const&)            = delete;
		Window& operator=(Window const&) = delete;
		Window& operator=(Window&&);
		~Window();

		constexpr Window(Window&& rhs) noexcept : ptr{std::exchange(rhs.ptr, nullptr)}
		{ }

		constexpr Window(SDL_Window* myPtr) noexcept : ptr{myPtr}
		{ }

		Window(std::string_view title, size_t width, size_t height, WindowFlags flags = 
			WindowFlags::Default);

	public:
		size_t GetWidth() const;
		size_t GetHeight() const;
		// {width, height}
		// This is more efficient than calling GetWidth and GetHeight separately.
		WindowSize GetSize() const;
		std::string_view GetTitle() const;
		Vec2 GetPosition() const;
		// {width / 2, height / 2}
		Vec2 GetCenter() const;
		// {0, 0, width, height}
		Rect GetRect() const;
		// {0, 0, width, height}
		RectF GetFRect() const;
		bool IsResizeable() const;
		bool IsAlwaysOnTop() const;
		bool IsMaximized() const;
		bool IsMinimized() const;
		bool IsBorderless() const;
		FullScreenMode GetFullScreen() const;

	public:
		void SetSize(size_t newWidth, size_t newHeight);
		void SetTitle(std::string_view newTitle);
		void SetPosition(Vec2 const& newPos);
		void SetResizeable(bool toWhat) const;
		void SetAlwaysOnTop(bool toWhat) const;
		void Maximize() const;
		void Minimize() const;
		void SetBorderless(bool toWhat) const;
		void SetFullScreen(FullScreenMode toWhat);

		void ShowCursor(bool toggle);
		void ShowMessageBox(MessageBoxFlag flag, std::string_view title, std::string_view message);

	public:
		SDL_Window* ptr{};
	};
}