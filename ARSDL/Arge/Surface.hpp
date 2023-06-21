#pragma once

#include "ArgeCore.hpp"
#include "Color.hpp"
#include "Enums.hpp"
#include "RectF.hpp"
#include "Rect.hpp"

#include <SDL/SDL.h>

namespace Arge {
	class Surface
	{
	public:
		Surface(Surface const& rhs)            = default;
		Surface& operator=(Surface const& rhs) = default;
		Surface& operator=(Surface&& rhs) noexcept;
		~Surface();

		constexpr Surface(SDL_Surface* myPtr) noexcept : ptr{myPtr}
		{
		}

		constexpr Surface(Surface&& rhs) noexcept : ptr{std::exchange(rhs.ptr, nullptr)}
		{
		}

		Surface(size_t width, size_t height, size_t pixelDepth = 32U);

	private:
		Surface() = default;

	public:
		static Surface FromFile(std::string_view fileName);

	public:
		// Surface must be locked before modification.
		void Lock();
		void Unlock();
		// Has bound checking, returns true if within bounds.
		bool PutPixel(size_t x, size_t y, Color color);
		// Has no bound checking in release.
		void PutPixelUnchecked(size_t x, size_t y, Color color);
		void FillRect(size_t x, size_t y, size_t w, size_t h, Color color);
		void FillRect(Rect const& rect, Color color);
		void FillRects(Rect const* rects, size_t count, Color color);

		Surface Doublicate() const;
		void SaveToFile(std::string_view fileName);

	public:
		bool HasRLE() const;
		void SetRLE(bool toWhat);

		void SetColorKey(Color color);
		void DisableColorKey();
		Color GetColorKey() const; // Throws when there's none.

		void SetBlendMode(BlendMode toWhat);
		BlendMode GetBlendMode() const;

		void SetClipRect(Rect const& rect);
		void DisableClipRect();
		Rect GetClipRect(); // Throws when there's none.

	public:
#ifdef ARGE_DEBUG_MODE
		bool m_bLocked{};
#endif
		SDL_Surface* ptr{};
	};
}