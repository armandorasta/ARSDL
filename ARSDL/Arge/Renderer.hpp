#pragma once
#include "ArgeCore.hpp"
#include "Vec2.hpp"
#include "Color.hpp"
#include "Texture.hpp"
#include "Enums.hpp"
#include "RectF.hpp"
#include "Rect.hpp"
#include "Vertex.hpp"

#include <SDL.h>

namespace Arge {
	class Window;

	class Renderer
	{
	public:
		Renderer(Window& window);
		Renderer(Renderer const&)                    = default;
		Renderer& operator=(Renderer const&)         = default;
		Renderer& operator=(Renderer&& rhs) noexcept;
		~Renderer();

		constexpr Renderer(SDL_Renderer* myPtr) noexcept : ptr{myPtr}
		{
		}

		constexpr Renderer(Renderer&& rhs) noexcept : ptr{std::exchange(rhs.ptr, nullptr)}
		{
		}

	public:
		void Clear(Color color = 0);

		void DrawPoint(Vec2 const& pos, Color color);
		void DrawLine(Vec2 const& v0, Vec2 const& v1, Color color, float thick = 1.0f);
		void DrawRect(Vec2 const& pos, float w, float h, Color color, float thick = 1.0f);
		void DrawRect(RectF const& rect, Color color, float thick = 1.0f);
		void FillRect(Vec2 const& pos, float w, float h, Color color);
		void FillRect(RectF const& rect, Color color);
		void DrawModel(std::vector<Vec2> const& points, Color color, float thick = 1.0f);
		void FillModel(std::vector<Vec2> const& points, Color color);
		void DrawCircle(Vec2 const& center, float r, Color color, float thick = 1.0f);
		void FillCircle(Vec2 const& center, float r, Color color);
		void DrawPolygon(Vec2 const& center, float r, size_t sideCount, Color color, 
			float thick = 1.0f, float rotation = 0.0f);
		void FillPolygon(Vec2 const& center, float r, size_t sideCount, Color color, 
			float rotation = 0.0f);

		void DrawTexture(Texture& tex, RectF const& destRect, Rect const& srcRect);
		void DrawTexture(Texture& tex, RectF const& destRect);

		void RenderOnGPU(std::vector<ColorVertex> const& verts);
		void RenderOnGPU(std::vector<ColorVertex> const& verts, std::vector<int> const& inds);
		void RenderOnGPU(std::vector<TexVertex> const& verts, std::vector<int> const& inds,
			Texture& tex);

		// TODO...
		// void CopyFromEx(...);

		void Present();

		void SetBlendMode(BlendMode mode);
		BlendMode GetBlendMode();

		bool HasSupportForRenderTargets() const;
		void SetRenderTarget(Texture newTarget);
		Texture GetRenderTarget();

		void SetClipRect(Rect const& toWhat);
		Rect GetClipRect() const;
		void DisableClipping();
		bool IsClippingEnabled();

		void SetScale(float scaleX, float scaleY);
		Vec2 GetScale() const;

		void SetVSync(bool toWhat);

	private:
		void SetColor(Color toWhat);

	public:
		SDL_Renderer* ptr{};
	};
}