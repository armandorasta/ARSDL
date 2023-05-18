#pragma once
#include "STD.hpp"
#include "FPoint.hpp"
#include "Color.hpp"
#include "Texture.hpp"
#include "Enums.hpp"
#include "FRect.hpp"
#include "Rect.hpp"
#include "Vertex.hpp"

#include <SDL.h>

namespace ArSDL {
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

		void DrawPoint(FPoint const& pos, Color color);
		void DrawLine(FPoint const& v0, FPoint const& v1, Color color, float thick = 1.0f);
		void DrawRect(FPoint const& pos, float w, float h, Color color, float thick = 1.0f);
		void DrawRect(FRect const& rect, Color color, float thick = 1.0f);
		void FillRect(FPoint const& pos, float w, float h, Color color);
		void FillRect(FRect const& rect, Color color);
		void DrawModel(std::vector<FPoint> const& points, Color color, float thick = 1.0f);
		void FillModel(std::vector<FPoint> const& points, Color color);
		void DrawCircle(FPoint const& center, float r, Color color, float thick = 1.0f);
		void FillCircle(FPoint const& center, float r, Color color);
		void DrawPolygon(FPoint const& center, float r, size_t sideCount, Color color, 
			float thick = 1.0f, float rotation = 0.0f);
		void FillPolygon(FPoint const& center, float r, size_t sideCount, Color color, 
			float rotation = 0.0f);

		void DrawTexture(Texture& tex, FRect const& destRect, Rect const& srcRect);
		void DrawTexture(Texture& tex, FRect const& destRect);

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
		FPoint GetScale() const;

		void SetVSync(bool toWhat);

	private:
		void SetColor(Color toWhat);

	public:
		SDL_Renderer* ptr{};
	};
}