#pragma once
#include "ArgeCore.hpp"
#include "Vec2.hpp"
#include "RectF.hpp"
#include "Renderer.hpp"

namespace Arge {
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
		constexpr Camera(Window const& window, Vec2 const& translation, float scale = 1.0f)
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

		constexpr Vec2 const& GetTranslation() const
		{
			return m_Translation;
		}

		constexpr Camera& SetTranslation(Vec2 const& toWhat)
		{
			m_Translation = toWhat;
			return *this;
		}

		constexpr Camera& TranslateBy(Vec2 const& delta)
		{
			m_Translation += delta;
			return *this;
		}

		constexpr Window const& GetWindow() const
		{
			return *m_pWin;
		}

		// Gives a rectangle that shows the bounds of what it could see.
		RectF GetRect(Window const& screen) const;

		// Transforms a vector in screen-space to camera-space.
		Vec2 operator()(Vec2 const& vec) const;
		Vec2 Trans(Vec2 const& vec) const;

		// Transforms a vector in camera-space to screen-space.
		Vec2 operator[](Vec2 const& vec) const;
		Vec2 RevTrans(Vec2 const& vec) const;


		void DrawPoint(Renderer& gfx, Vec2 const& pos, Color color) const
		{
			gfx.DrawPoint(Trans(pos), color);
		}

		void DrawLine(Renderer& gfx, Vec2 const& v0, Vec2 const& v1, Color color, float thick = 1.0f) const
		{
			gfx.DrawLine(Trans(v0), Trans(v1), color, thick);
		}

		void DrawRect(Renderer& gfx, Vec2 const& pos, float w, float h, Color color, float thick = 1.0f) const
		{
			gfx.DrawRect(Trans(pos), w * GetScale(), h * GetScale(), color, thick);
		}

		void DrawRect(Renderer& gfx, RectF const& rect, Color color, float thick = 1.0f) const
		{
			DrawRect(gfx, {rect.x, rect.y}, rect.w, rect.h, color, thick);
		}

		void FillRect(Renderer& gfx, Vec2 const& pos, float w, float h, Color color) const
		{
			gfx.FillRect(Trans(pos), w * GetScale(), h * GetScale(), color);
		}

		void FillRect(Renderer& gfx, RectF const& rect, Color color) const
		{
			FillRect(gfx, {rect.x, rect.y}, rect.w, rect.h, color);
		}

		void DrawModel(Renderer& gfx, std::vector<Vec2> points, Color color, float thick = 1.0f) const
		{
			for (auto& point : points)
			{
				point = Trans(point);
			}

			gfx.DrawModel(points, color, thick);
		}

		void FillModel(Renderer& gfx, std::vector<Vec2> points, Color color) const
		{
			for (auto& point : points)
			{
				point = Trans(point);
			}

			gfx.DrawModel(points, color);
		}

		void DrawCircle(Renderer& gfx, Vec2 const& center, float r, Color color, float thick = 1.0f) const
		{
			gfx.DrawCircle(Trans(center), r * GetScale(), color, thick);
		}

		void FillCircle(Renderer& gfx, Vec2 const& center, float r, Color color) const
		{
			gfx.FillCircle(Trans(center), r * GetScale(), color);
		}

		void DrawPolygon(Renderer& gfx, Vec2 const& center, float r, size_t sideCount, Color color, 
			float thick = 1.0f, float rotation = 0.0f) const
		{
			gfx.DrawPolygon(Trans(center), r * GetScale(), sideCount, color, thick, rotation);
		}

		void FillPolygon(Renderer& gfx, Vec2 const& center, float r, size_t sideCount, Color color, 
			float rotation = 0.0f) const
		{
			gfx.FillPolygon(Trans(center), r * GetScale(), sideCount, color, rotation);
		}
		
	private:
		Window const* m_pWin;
		Vec2 m_Translation{};
		float m_Scale{1.0f};
	};
}