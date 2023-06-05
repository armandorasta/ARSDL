#pragma once
#include "STD.hpp"
#include "FPoint.hpp"
#include "FRect.hpp"

namespace ArSDL {
	class Window;

	class Camera
	{
	public:
		constexpr Camera(Camera const&)            = default;
		constexpr Camera(Camera&&)                 = default;
		constexpr Camera& operator=(Camera const&) = default;
		constexpr Camera& operator=(Camera&&)      = default;

		constexpr Camera(Window const& window) : m_pWin{&window} { }
		constexpr Camera(Window const& window, FPoint const& translation, float scale = 1.0f)
			: m_pWin{&window}, m_Translation{translation}, m_Scale{scale}
		{ }


		constexpr float GetScale() const
		{
			return m_Scale;
		}

		constexpr Camera& SetScale(float toWhat)
		{
			m_Scale = toWhat;
			return *this;
		}

		constexpr Camera& ScaleBy(float percent)
		{
			m_Scale *= percent;
			return *this;
		}

		constexpr FPoint const& GetTranslation() const
		{
			return m_Translation;
		}

		constexpr Camera& SetTranslation(FPoint const& toWhat)
		{
			m_Translation = toWhat;
			return *this;
		}

		constexpr Camera& TranslateBy(FPoint const& delta)
		{
			m_Translation += delta;
			return *this;
		}

		constexpr Window const& GetWindow() const
		{
			return *m_pWin;
		}

		// Gives a rectangle that shows the bounds of what it could see.
		FRect GetRect(Window const& screen) const;

		// Transforms a vector in screen-space to camera-space.
		FPoint operator()(FPoint const& vec) const;
		FPoint Trans(FPoint const& vec) const;

		// Transforms a vector in camera-space to screen-space.
		FPoint operator[](FPoint const& vec) const;
		FPoint RevTrans(FPoint const& vec) const;

	private:
		Window const* m_pWin;
		FPoint m_Translation{};
		float m_Scale{1.0f};
	};
}