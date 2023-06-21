#include "pch.h"
#include "Event.hpp"

namespace Arge {
	WindowEvent::WindowEvent(SDL_WindowEvent const& rhs)
	{
		std::memcpy(this, &rhs, sizeof WindowEvent);
	}

	KeyboardEvent::KeyboardEvent(SDL_KeyboardEvent const& rhs)
	{
		std::memcpy(this, &rhs, sizeof KeyboardEvent);
	}

	MouseMotionEvent::MouseMotionEvent(SDL_MouseMotionEvent const& rhs)
	{
		std::memcpy(this, &rhs, sizeof MouseMotionEvent);
	}

	MouseButtonEvent::MouseButtonEvent(SDL_MouseButtonEvent const& rhs)
	{
		std::memcpy(this, &rhs, sizeof MouseButtonEvent);
	}

	MouseWheelEvent::MouseWheelEvent(SDL_MouseWheelEvent const& rhs)
	{
		std::memcpy(this, &rhs, sizeof MouseWheelEvent);
	}

	WindowEvent Event::AsWindowEvent() const
	{
		ARGE_ERROR_DEBUG_THROW_IF(GetType() != EventType::WindowEvent);
		return m_ev.window;
	}

	KeyboardEvent Event::AsKeyboardEvent() const
	{
#ifdef ARGE_DEBUG_MODE
		auto const type{GetType()}; // Only used in debug mode.
#endif
		ARGE_ERROR_DEBUG_THROW_IF(type != EventType::KeyDown && type != EventType::KeyUp);
		return m_ev.key;
	}

	MouseMotionEvent Event::AsMouseMotionEvent() const
	{
		ARGE_ERROR_DEBUG_THROW_IF(GetType() != EventType::MouseMotion);
		return m_ev.motion;
	}

	MouseButtonEvent Event::AsMouseButtonEvent() const
	{
#ifdef ARGE_DEBUG_MODE
		auto const type{GetType()};
#endif
		ARGE_ERROR_DEBUG_THROW_IF(type != EventType::MouseButtonDown
		                        && type != EventType::MouseButtonUp);
		return m_ev.button;
	}

	MouseWheelEvent Event::AsMouseWheelEvent() const
	{
		ARGE_ERROR_DEBUG_THROW_IF(GetType() != EventType::MouseWheel);
		return m_ev.wheel;
	}
}
