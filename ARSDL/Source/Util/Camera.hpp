#pragma once
#include "STD.hpp"
#include "FPoint.hpp"
#include "FRect.hpp"
#include "Renderer.hpp"

namespace ArSDL {
	class Window;
	class Renderer;

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


		void DrawPoint(Renderer& gfx, FPoint const& pos, Color color)
		{
			gfx.DrawPoint(Trans(pos), color);
		}

		void DrawLine(Renderer& gfx, FPoint const& v0, FPoint const& v1, Color color, float thick = 1.0f)
		{
			gfx.DrawLine(Trans(v0), Trans(v1), color, thick);
		}

		void DrawRect(Renderer& gfx, FPoint const& pos, float w, float h, Color color, float thick = 1.0f)
		{
			gfx.DrawRect(Trans(pos), w * GetScale(), h * GetScale(), color, thick);
		}

		void DrawRect(Renderer& gfx, FRect const& rect, Color color, float thick = 1.0f)
		{
			DrawRect(gfx, {rect.x, rect.y}, rect.w, rect.h, color, thick);
		}

		void FillRect(Renderer& gfx, FPoint const& pos, float w, float h, Color color)
		{
			gfx.FillRect(Trans(pos), w * GetScale(), h * GetScale(), color);
		}

		void FillRect(Renderer& gfx, FRect const& rect, Color color)
		{
			FillRect(gfx, {rect.x, rect.y}, rect.w, rect.h, color);
		}

		void DrawModel(Renderer& gfx, std::vector<FPoint> points, Color color, float thick = 1.0f)
		{
			for (auto& point : points)
			{
				point = Trans(point);
			}

			gfx.DrawModel(points, color, thick);
		}

		void FillModel(Renderer& gfx, std::vector<FPoint> points, Color color)
		{
			for (auto& point : points)
			{
				point = Trans(point);
			}

			gfx.DrawModel(points, color);
		}

		void DrawCircle(Renderer& gfx, FPoint const& center, float r, Color color, float thick = 1.0f)
		{
			gfx.DrawCircle(Trans(center), r * GetScale(), color, thick);
		}

		void FillCircle(Renderer& gfx, FPoint const& center, float r, Color color)
		{
			gfx.FillCircle(Trans(center), r * GetScale(), color);
		}

		void DrawPolygon(Renderer& gfx, FPoint const& center, float r, size_t sideCount, Color color,
			float thick = 1.0f, float rotation = 0.0f)
		{
			gfx.DrawPolygon(Trans(center), r * GetScale(), sideCount, color, thick, rotation);
		}

		void FillPolygon(Renderer& gfx, FPoint const& center, float r, size_t sideCount, Color color,
			float rotation = 0.0f)
		{
			gfx.FillPolygon(Trans(center), r * GetScale(), sideCount, color, rotation);
		}
		
	private:
		Window const* m_pWin;
		FPoint m_Translation{};
		float m_Scale{1.0f};
	};
}