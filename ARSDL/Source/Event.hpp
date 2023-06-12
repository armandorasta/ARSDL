#pragma once
#include "STD.hpp"
#include "Enums.hpp"
#include "ArSDLError.hpp"
#include "Vec2.hpp"

#include <SDL.h>

namespace Arge {
	class WindowEvent : SDL_WindowEvent
	{
	public:
		WindowEvent(SDL_WindowEvent const& rhs);

	public:
		constexpr SDL_WindowEvent& GetRaw()
		{
			return static_cast<SDL_WindowEvent&>(*this);
		}

		constexpr WindowEventID GetEventID() const
		{
			return static_cast<WindowEventID>(event);
		}

		// Used for WindowEventID::Moved
		constexpr Vec2 GetNewLocation()
		{
			ARSDL_ERROR_DEBUG_THROW_IF(event != SDL_WINDOWEVENT_MOVED);
			return {static_cast<float>(data1), static_cast<float>(data2)};
		}

		// Used for WindowEventID::Resized
		constexpr WindowSize GetNewSize()
		{
			ARSDL_ERROR_DEBUG_THROW_IF(event != SDL_WINDOWEVENT_RESIZED);
			return {static_cast<size_t>(data1), static_cast<size_t>(data2)};
		}
	};

	class KeyboardEvent : SDL_KeyboardEvent
	{
	public:
		KeyboardEvent(SDL_KeyboardEvent const& rhs);

		constexpr bool operator==(Keycode kc) const
		{
			return GetKeyCode() == kc;
		}

	public:
		constexpr bool IsPressed() const
		{
			return static_cast<bool>(state);
		}

		constexpr bool HasRepeated() const
		{
			return static_cast<bool>(repeat);
		}

		constexpr Keycode GetKeyCode() const
		{
			return static_cast<Keycode>(keysym.sym);
		}

		constexpr Keymod GetKeyModifier() const
		{
			return static_cast<Keymod>(keysym.mod);
		}
	};

	class MouseMotionEvent : SDL_MouseMotionEvent
	{
	public:
		MouseMotionEvent(SDL_MouseMotionEvent const& rhs);

	public:
		constexpr bool IsLeftPressed() const
		{
			return (state & SDL_BUTTON_LMASK) != 0;
		}

		constexpr bool IsMidPressed() const
		{
			return (state & SDL_BUTTON_MMASK) != 0;
		}

		constexpr bool IsRightPressed() const
		{
			return (state & SDL_BUTTON_RMASK) != 0;
		}

		constexpr Vec2 GetPos() const
		{
			return {static_cast<float>(x), static_cast<float>(y)};
		}

		constexpr Vec2 GetRelativePos() const
		{
			return {static_cast<float>(xrel), static_cast<float>(yrel)};
		}
	};

	class MouseButtonEvent : SDL_MouseButtonEvent
	{
	public:
		MouseButtonEvent(SDL_MouseButtonEvent const& rhs);

	public:
		constexpr bool IsPressed() const
		{
			return state != 0;
		}

		constexpr size_t GetClickCount() const
		{
			return static_cast<size_t>(clicks);
		}

		constexpr size_t GetIndex() const
		{
			return button;
		}

		constexpr bool IsLeft() const
		{
			return button == 1;
		}

		constexpr bool IsMid() const
		{
			return button == 2;
		}

		constexpr bool IsRight() const
		{
			return button == 3;
		}

		constexpr Vec2 GetPos() const
		{
			return {static_cast<float>(x), static_cast<float>(y)};
		}
	};

	class MouseWheelEvent : SDL_MouseWheelEvent
	{
	public:
		MouseWheelEvent(SDL_MouseWheelEvent const& rhs);

	public:
		constexpr bool IsDirectionFlipped() const
		{
			return direction == SDL_MOUSEWHEEL_FLIPPED;
		}

		constexpr int32_t GetVertical() const
		{
			return y;
		}

		constexpr int32_t GetHorizontal() const
		{
			return x;
		}

		constexpr float GetVerticalPrecise() const
		{
			return preciseY;
		}

		constexpr float GetHorizontalPrecise() const
		{
			return preciseX;
		}

		constexpr Vec2 GetPos() const
		{
			return {static_cast<float>(mouseX), static_cast<float>(mouseY)};
		}
	};

	class Event 
	{
	public:
		constexpr Event() = default;

	public:
		constexpr SDL_Event& GetRawAccess()
		{
			return m_ev;
		}

		constexpr SDL_Event const& GetRawAccess() const
		{
			return m_ev;
		}

		constexpr EventType GetType() const 
		{
			return static_cast<EventType>(m_ev.type);
		}

		// I am really hoping the complier will be able to see through the memcpy calls inside
		// the constructors of those wrappers and optimize them away.

		WindowEvent      AsWindowEvent()      const;
		KeyboardEvent    AsKeyboardEvent()    const;
		MouseMotionEvent AsMouseMotionEvent() const;
		MouseButtonEvent AsMouseButtonEvent() const;
		MouseWheelEvent  AsMouseWheelEvent()  const;

	private:
		SDL_Event m_ev;
	};
}