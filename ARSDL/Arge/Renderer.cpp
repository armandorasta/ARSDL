#include "pch.h"
#include "Renderer.hpp"

#include "ArSDLError.hpp"
#include "Color.hpp"
#include "Texture.hpp"
#include "Window.hpp"

namespace Arge {
	Renderer::Renderer(Window& window) :
		ptr{SDL_CreateRenderer(window.ptr, -1, SDL_RendererFlags::SDL_RENDERER_ACCELERATED)}
	{
		ARSDL_ERROR_HANDLE_NULL(ptr);
	}

	Renderer::~Renderer()
	{
		SDL_DestroyRenderer(ptr);
	}

	Renderer& Renderer::operator=(Renderer&& rhs) noexcept
	{
		if (&rhs != this)
		{
			SDL_DestroyRenderer(ptr);
			ptr = std::exchange(rhs.ptr, nullptr);
		}
		return *this;
	}

	void Renderer::Clear(Color color)
	{
		SetColor(color);
		ARSDL_ERROR_HANDLE_NEG(SDL_RenderClear(ptr));
	}

	void Renderer::DrawPoint(Vec2 const& pos, Color color)
	{
		SetColor(color);
		ARSDL_ERROR_HANDLE_NEG(SDL_RenderDrawPointF(ptr, pos.x, pos.y));
	}

	void Renderer::DrawLine(Vec2 const& v0, Vec2 const& v1, Color color, float thick)
	{
		SetColor(color);
		if (std::abs(thick - 1.0f) < 0.0000001f)
		{
			ARSDL_ERROR_HANDLE_NEG(SDL_RenderDrawLineF(ptr, v0.x, v0.y, v1.x, v1.y));
			return;
		}

		auto const vDiff{v1 - v0};
		auto const vLeft{vDiff.CounterClockwiseNormal().Normalized() * thick};

		/*
			0                     1
			*=====================*
			|#####################|
			*=====================*
			3                     2
		*/

		std::array<SDL_Vertex, 4> const verts{{
			{v0 + vLeft, color},
			{v1 + vLeft, color},
			{v1 - vLeft, color},
			{v0 - vLeft, color},
		}};
		std::array constexpr const inds{0, 1, 2, 2, 3, 0};
		ARSDL_ERROR_HANDLE_NEG(SDL_RenderGeometry(ptr, nullptr, verts.data(),
			static_cast<int>(verts.size()), inds.data(), static_cast<int>(inds.size())));
	}

	void Renderer::DrawRect(Vec2 const& pos, float w, float h, Color color, float thick)
	{
		DrawRect({pos.x, pos.y, w, h}, color, thick);
	}

	void Renderer::DrawRect(RectF const& rect, Color color, float thick)
	{
		if (std::abs(thick - 1.0f) < 0.0000001f)
		{
			SetColor(color);
			ARSDL_ERROR_HANDLE_NEG(SDL_RenderDrawRectF(ptr, &rect));
			return;
		}
		else if (thick >= std::min(*&rect.w, *&rect.h))
		{
			FillRect(*&rect, color);
			return;
		}

		SetColor(color);
		auto const vInnerTL = Vec2{rect.x + thick, rect.y + thick};
		auto const vOuterTL = Vec2{rect.x - thick, rect.y - thick};
		auto const fInnerWidth {rect.w - 2.0f * thick};
		auto const fOuterWidth {rect.w + 2.0f * thick};
		auto const fInnerHeight{rect.h - 2.0f * thick};
		auto const fOuterHeight{rect.h + 2.0f * thick};

		/*
			1                              3
			*==============================*
			|   *======================*   |
			|   |0                    2|   |
			|   |                      |   |
			|   |                      |   |
			|   |                      |   |
			|   |6                    4|   |
			|   *======================*   |
			*==============================*
			7                              5
		*/

		std::array<SDL_Vertex, 8> const verts{{
			{vInnerTL, color}, // 
			{vOuterTL, color}, // Top left.
			{{vInnerTL.x + fInnerWidth, vInnerTL.y}, color}, // 
			{{vOuterTL.x + fOuterWidth, vOuterTL.y}, color}, // Top right.
			{{vInnerTL.x + fInnerWidth, vInnerTL.y + fInnerHeight}, color}, // 
			{{vOuterTL.x + fOuterWidth, vOuterTL.y + fOuterHeight}, color},	// Bottom right.
			{{vInnerTL.x, vInnerTL.y + fInnerHeight}, color}, // 
			{{vOuterTL.x, vOuterTL.y + fOuterHeight}, color}, // Bottom left.
		}};
		std::array constexpr inds{
			0, 1, 3, 3, 2, 0,
			2, 3, 5, 5, 4, 2,
			4, 5, 7, 7, 6, 4,
			6, 7, 1, 1, 0, 6,
		};
		ARSDL_ERROR_HANDLE_NEG(SDL_RenderGeometry(ptr, nullptr, verts.data(),
			static_cast<int>(verts.size()), inds.data(), static_cast<int>(inds.size())));
	}

	void Renderer::FillRect(Vec2 const& pos, float w, float h, Color color)
	{
		FillRect({pos.x, pos.y, w, h}, color);
	}

	void Renderer::FillRect(RectF const& rect, Color color)
	{
		SetColor(color);
		ARSDL_ERROR_HANDLE_NEG(SDL_RenderFillRectF(ptr, &rect));
	}

	void Renderer::DrawModel(std::vector<Vec2> const& points, Color color, float thick)
	{
		auto const size{points.size()};
		for (size_t i{}; i < size; ++i)
		{
			DrawLine(points[i], points[(i + 1) % size], color, thick);
		}
	}

	void Renderer::FillModel(std::vector<Vec2> const& points, Color color)
	{
		std::vector<SDL_Vertex> verts{};
		verts.reserve(points.size());
		std::ranges::transform(points, std::back_inserter(verts), [=](Vec2 const& vec) {
			return SDL_Vertex{
				.position{vec.x, vec.y},
				.color{color.r, color.g, color.b, color.a},
			};
		});
		ARSDL_ERROR_HANDLE_NEG(SDL_RenderGeometry(ptr, nullptr, verts.data(),
			static_cast<int>(verts.size()), nullptr, 0));
	}

	void Renderer::DrawCircle(Vec2 const& center, float r, Color color, float thick)
	{
		if (thick >= r)
		{
			FillCircle(center, r + thick, color);
		}
		else
		{
			constexpr auto Multiplier{10.0f};
			auto const steps{static_cast<std::size_t>(Multiplier * r)};
			DrawPolygon(center, r, steps, color, thick);
		}
	}

	void Renderer::FillCircle(Vec2 const& center, float r, Color color)
	{
		constexpr auto Multiplier{10.0f};
		auto const steps{static_cast<std::size_t>(Multiplier * r)};
		FillPolygon(center, r, steps, color);
	}

	void Renderer::DrawPolygon(Vec2 const& center, float r, size_t sideCount, Color color, float thick, 
		float rotation)
	{
		if (thick >= r)
		{
			FillCircle(center, r + thick, color);
			return;
		}

		auto const fAngle{2.f * 3.1415926535f / sideCount};
		std::vector<SDL_Vertex> verts{};
		verts.reserve(2 * sideCount);
		std::vector<int> inds{};
		inds.reserve(6 * sideCount);
		for (std::size_t i{}; i < sideCount; ++i)
		{
			auto dirVec = Vec2::FromDir(rotation + i * fAngle);
			verts.push_back({
				.position{center + dirVec * (r - thick)},
				.color{color},
				.tex_coord{}
			});
			verts.push_back({
				.position{center + dirVec * (r + thick)},
				.color{color},
				.tex_coord{}
			});
		}

		for (std::size_t i{}; i < sideCount - 1; ++i)
		{
			/*
					 i + 3
					 v
					 *================================* <- i + 1
					  *           rotation dir       *
					   *          <<<--------       *
						*                          *
				i + 2 -> *========================* <- i + 0
			*/

			auto const index{static_cast<int>(i)};
			inds.push_back(2 * index + 0);
			inds.push_back(2 * index + 2);
			inds.push_back(2 * index + 1);

			inds.push_back(2 * index + 1);
			inds.push_back(2 * index + 2);
			inds.push_back(2 * index + 3);
		}

		// Connecting the last edge with the first one.
		auto const lastIndex{static_cast<int>(2 * (sideCount - 1))};
		inds.push_back(lastIndex);
		inds.push_back(0);
		inds.push_back(lastIndex + 1);

		inds.push_back(lastIndex + 1);
		inds.push_back(0);
		inds.push_back(1);

		ARSDL_ERROR_HANDLE_NEG(SDL_RenderGeometry(ptr, nullptr, verts.data(),
			static_cast<int>(verts.size()), inds.data(), static_cast<int>(inds.size())));
	}

	void Renderer::FillPolygon(Vec2 const& center, float r, size_t sideCount, Color color, 
		float rotation)
	{
		auto const fAngle{2.f * 3.1415926535f / sideCount};
		std::vector<SDL_Vertex> verts{};
		verts.reserve(sideCount);
		verts.push_back({.position{center}, .color{color}, .tex_coord{}});
		std::vector<int> inds{};
		inds.reserve(3 * sideCount);
		for (std::size_t i{}; i < sideCount + 1; ++i)
		{
			verts.push_back({
				.position = center + Vec2::FromDir(rotation + i * fAngle, r),
				.color{color},
				.tex_coord{}
			});
			inds.push_back(0);
			inds.push_back(static_cast<int>(i));
			inds.push_back(static_cast<int>(i) + 1);
		}
		ARSDL_ERROR_HANDLE_NEG(SDL_RenderGeometry(ptr, nullptr, verts.data(),
			static_cast<int>(verts.size()), inds.data(), static_cast<int>(inds.size())));
	}

	void Renderer::DrawTexture(Texture& tex, RectF const& destRect, Rect const& srcRect)
	{
		ARSDL_ERROR_HANDLE_NEG(SDL_RenderCopyF(ptr, tex.ptr, &srcRect, &destRect));
	}

	void Renderer::DrawTexture(Texture& tex, RectF const& destRect)
	{
		ARSDL_ERROR_HANDLE_NEG(SDL_RenderCopyF(ptr, tex.ptr, nullptr, &destRect));
	}

	void Renderer::RenderOnGPU(std::vector<ColorVertex> const& verts)
	{ 
		SDL_RenderGeometry(ptr, nullptr, verts.data(), static_cast<int>(verts.size()), nullptr, 0);
	}

	void Renderer::RenderOnGPU(std::vector<ColorVertex> const& verts, std::vector<int> const& inds)
	{ 
		SDL_RenderGeometry(ptr, nullptr, verts.data(), static_cast<int>(verts.size()), 
			inds.data(), static_cast<int>(inds.size()));
	}

	void Renderer::RenderOnGPU(std::vector<TexVertex> const& verts, std::vector<int> const& inds, 
		Texture& tex)
	{ 
		SDL_RenderGeometry(ptr, tex.ptr, verts.data(), static_cast<int>(verts.size()),
			inds.data(), static_cast<int>(inds.size()));
	}

	void Renderer::Present()
	{
		SDL_RenderPresent(ptr);
	}

	void Renderer::SetBlendMode(BlendMode mode)
	{
		ARSDL_ERROR_HANDLE_NEG(SDL_SetRenderDrawBlendMode(ptr, static_cast<SDL_BlendMode>(mode)));
	}

	BlendMode Renderer::GetBlendMode()
	{
		SDL_BlendMode mode{};
		ARSDL_ERROR_HANDLE_NEG(SDL_GetRenderDrawBlendMode(ptr, &mode));
		return static_cast<BlendMode>(mode);
	}

	bool Renderer::HasSupportForRenderTargets() const
	{
		return SDL_RenderTargetSupported(ptr) == SDL_TRUE ? true : false;
	}

	void Renderer::SetRenderTarget(Texture newTarget)
	{
		ARSDL_ERROR_HANDLE_NEG(SDL_SetRenderTarget(ptr, newTarget.ptr));
	}

	Texture Renderer::GetRenderTarget()
	{
		return {SDL_GetRenderTarget(ptr)};
	}

	void Renderer::SetClipRect(Rect const& toWhat)
	{
		ARSDL_ERROR_HANDLE_NEG(SDL_RenderSetClipRect(ptr, &toWhat));
	}

	Rect Renderer::GetClipRect() const
	{
		Rect res{};
		SDL_RenderGetClipRect(ptr, &res);
		return res;
	}

	void Renderer::DisableClipping()
	{
		ARSDL_ERROR_HANDLE_NEG(SDL_RenderSetClipRect(ptr, nullptr));
	}

	bool Renderer::IsClippingEnabled()
	{
		return SDL_RenderIsClipEnabled(ptr) == SDL_TRUE ? true : false;
	}

	void Renderer::SetScale(float scaleX, float scaleY)
	{
		ARSDL_ERROR_HANDLE_NEG(SDL_RenderSetScale(ptr, scaleX, scaleY));
	}

	Vec2 Renderer::GetScale() const
	{
		Vec2 res;
		SDL_RenderGetScale(ptr, &res.x, &res.y);
		return res;
	}

	void Renderer::SetVSync(bool toWhat)
	{
		SDL_RenderSetVSync(ptr, toWhat ? 1 : 0);
	}

	void Renderer::SetColor(Color toWhat)
	{
		ARSDL_ERROR_HANDLE_NEG(SDL_SetRenderDrawColor(ptr, toWhat.r, toWhat.g, toWhat.b, toWhat.a));
	}
}
