#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

namespace ArSDL {
	enum class BlendMode 
	{
		Off            = SDL_BLENDMODE_NONE,
		Normal         = SDL_BLENDMODE_BLEND,
		Additive       = SDL_BLENDMODE_ADD,
		Mulitplicative = SDL_BLENDMODE_MUL,
		Modular        = SDL_BLENDMODE_MOD,
	};

	enum class TextureAccess 
	{
		Static    = SDL_TEXTUREACCESS_STATIC,
		Streaming = SDL_TEXTUREACCESS_STREAMING,
		Target    = SDL_TEXTUREACCESS_TARGET,
	};

	enum class ScaleMode 
	{
		Nearest = SDL_ScaleModeNearest,
		Linear  = SDL_ScaleModeLinear,
		Best    = SDL_ScaleModeBest,
	};

	struct WindowSize
	{
		size_t width;
		size_t height;
	};

	enum class WindowFlags
	{
		Default     = 0,
		Maximized   = SDL_WINDOW_MINIMIZED,
		Minimized   = SDL_WINDOW_MAXIMIZED,
		FullScreen  = SDL_WINDOW_FULLSCREEN,
		AlwaysOnTop = SDL_WINDOW_ALWAYS_ON_TOP,
		Borderless  = SDL_WINDOW_BORDERLESS,
	};

	constexpr WindowFlags operator|(WindowFlags lhs, WindowFlags rhs)
	{
		using UType = std::underlying_type_t<WindowFlags>;
		return static_cast<WindowFlags>(static_cast<UType>(lhs) | static_cast<UType>(rhs));
	}

	constexpr WindowFlags operator&(WindowFlags lhs, WindowFlags rhs)
	{
		using UType = std::underlying_type_t<WindowFlags>;
		return static_cast<WindowFlags>(static_cast<UType>(lhs) & static_cast<UType>(rhs));
	}

	constexpr WindowFlags operator~(WindowFlags operand)
	{
		using UType = std::underlying_type_t<WindowFlags>;
		return static_cast<WindowFlags>(~static_cast<UType>(operand));
	}

	enum class FullScreenMode
	{
		Real = SDL_WINDOW_FULLSCREEN,
		Fake = SDL_WINDOW_FULLSCREEN_DESKTOP,
		Off  = 0,
	};

	enum class TextDirection
	{
		LeftToRight = TTF_DIRECTION_LTR,
		RightToLeft = TTF_DIRECTION_RTL,
		TopToBottom = TTF_DIRECTION_TTB,
		BottomToTop = TTF_DIRECTION_BTT,
	};

	enum EventType 
	{
		Quit            = SDL_QUIT,
		Terminate       = SDL_APP_TERMINATING,
		DisplayEvent    = SDL_DISPLAYEVENT,
		WindowEvent     = SDL_WINDOWEVENT,
		KeyUp           = SDL_KEYUP,
		KeyDown         = SDL_KEYDOWN,
		MouseMotion     = SDL_MOUSEMOTION,
		MouseButtonUp   = SDL_MOUSEBUTTONUP,
		MouseButtonDown = SDL_MOUSEBUTTONDOWN,
		MouseWheel      = SDL_MOUSEWHEEL,
	};

	enum EventCategory
	{
		App,
		Keyboard,
		Mouse,
	};

	constexpr EventCategory GetEventCategory(EventType evType)
	{
		switch (evType) {
		case Quit:
		case Terminate:
		case WindowEvent:
		case DisplayEvent:
			return EventCategory::App;
		case KeyUp:
		case KeyDown:
			return EventCategory::Keyboard;
		case MouseMotion:
		case MouseButtonUp:
		case MouseButtonDown:
		case MouseWheel:
			return EventCategory::Mouse;
		default:
			return EventCategory::App;
		}
	}

	enum MessageBoxFlag
	{
		Info    = SDL_MESSAGEBOX_INFORMATION,
		Warning = SDL_MESSAGEBOX_WARNING,
		Error   = SDL_MESSAGEBOX_ERROR,
	};

	// The original SDL enum contains some keys that are not present here; I did not include them 
	// either because I do not see them in my keyboard, or because they can be looked up directly 
	// by using their glyph.
	enum class KeyCode
	{
		Return         = SDLK_RETURN,
		Escape         = SDLK_ESCAPE,
		Backspace      = SDLK_BACKSPACE,
		Tab            = SDLK_TAB,
		Space          = SDLK_SPACE,
		CapsLock       = SDLK_CAPSLOCK,
		PrintScreen    = SDLK_PRINTSCREEN,
		ScrollLock     = SDLK_SCROLLLOCK,
		Pause          = SDLK_PAUSE,
		Insert         = SDLK_INSERT,
		Home           = SDLK_HOME,
		PageUp         = SDLK_PAGEUP,
		Delete         = SDLK_DELETE,
		End            = SDLK_END,
		PageDown       = SDLK_PAGEDOWN,
		Right          = SDLK_RIGHT,
		Left           = SDLK_LEFT,
		Down           = SDLK_DOWN,
		Up             = SDLK_UP,
		Application    = SDLK_APPLICATION,
		Power          = SDLK_POWER,
		Execute        = SDLK_EXECUTE,
		Help           = SDLK_HELP,
		Menu           = SDLK_MENU,
		LeftCtrl       = SDLK_LCTRL,
		LeftShift      = SDLK_LSHIFT,
		LeftAlt        = SDLK_LALT,
		LeftGui        = SDLK_LGUI,
		RightCtrl      = SDLK_RCTRL,
		RightShift     = SDLK_RSHIFT,
		RightAlt       = SDLK_RALT,
		RightGui       = SDLK_RGUI,

		Num0           = SDLK_0,
		Num1           = SDLK_1,
		Num2           = SDLK_2,
		Num3           = SDLK_3,
		Num4           = SDLK_4,
		Num5           = SDLK_5,
		Num6           = SDLK_6,
		Num7           = SDLK_7,
		Num8           = SDLK_8,
		Num9           = SDLK_9,

		F1             = SDLK_F1,
		F2             = SDLK_F2,
		F3             = SDLK_F3,
		F4             = SDLK_F4,
		F5             = SDLK_F5,
		F6             = SDLK_F6,
		F7             = SDLK_F7,
		F8             = SDLK_F8,
		F9             = SDLK_F9,
		F10            = SDLK_F10,
		F11            = SDLK_F11,
		F12            = SDLK_F12,
		F13            = SDLK_F13,
		F14            = SDLK_F14,
		F15            = SDLK_F15,
		F16            = SDLK_F16,
		F17            = SDLK_F17,
		F18            = SDLK_F18,
		F19            = SDLK_F19,
		F20            = SDLK_F20,
		F21            = SDLK_F21,
		F22            = SDLK_F22,
		F23            = SDLK_F23,
		F24            = SDLK_F24,

		NumLockClear   = SDLK_NUMLOCKCLEAR,
		KeypadDivide   = SDLK_KP_DIVIDE,
		KeypadMultiply = SDLK_KP_MULTIPLY,
		KeypadMinus    = SDLK_KP_MINUS,
		KeypadPlus     = SDLK_KP_PLUS,
		KeypadEnter    = SDLK_KP_ENTER,
		Keypad1        = SDLK_KP_1,
		Keypad2        = SDLK_KP_2,
		Keypad3        = SDLK_KP_3,
		Keypad4        = SDLK_KP_4,
		Keypad5        = SDLK_KP_5,
		Keypad6        = SDLK_KP_6,
		Keypad7        = SDLK_KP_7,
		Keypad8        = SDLK_KP_8,
		Keypad9        = SDLK_KP_9,
		Keypad0        = SDLK_KP_0,
		KeypadPeriod   = SDLK_KP_PERIOD,
		KeypadEquals   = SDLK_KP_EQUALS,
	};

	enum class Keymode
	{
		None       = KMOD_NONE,
		Leftshift  = KMOD_LSHIFT,
		Rightshift = KMOD_RSHIFT,
		Leftctrl   = KMOD_LCTRL,
		Rightctrl  = KMOD_RCTRL,
		Leftalt    = KMOD_LALT,
		Rightalt   = KMOD_RALT,
		Leftgui    = KMOD_LGUI,
		Rightgui   = KMOD_RGUI,
		Num        = KMOD_NUM,
		Caps       = KMOD_CAPS,
		Mode       = KMOD_MODE,
		Scroll     = KMOD_SCROLL,
		Ctrl       = KMOD_CTRL,
		Shift      = KMOD_SHIFT,
		Alt        = KMOD_ALT,
		Gui        = KMOD_GUI,
	};
}