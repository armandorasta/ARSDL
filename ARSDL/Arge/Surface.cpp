#include "pch.h"
#include "Surface.hpp"
#include "Color.hpp"
#include "ArSDLError.hpp"
#include "Rect.hpp"

#include <SDL/SDL_image.h>

namespace Arge {
	Surface& Surface::operator=(Surface&& rhs) noexcept
	{
		if (&rhs != this)
		{
			SDL_FreeSurface(ptr);
			ptr = rhs.ptr;
			rhs.ptr = nullptr;
		}
		return *this;
	}

	Arge::Surface::~Surface()
	{
		SDL_FreeSurface(ptr);
	}

	Surface::Surface(size_t width, size_t height, size_t pixelDepth) :
		ptr{SDL_CreateRGBSurface(
			0U,
			static_cast<int>(width),
			static_cast<int>(height),
			static_cast<int>(pixelDepth),
			Color::GetRedMask(),
			Color::GetGreenMask(),
			Color::GetBlueMask(),
			Color::GetAlphaMask()
		)}
	{
		ARGE_ERROR_HANDLE_NULL(ptr);
	}

	Surface Surface::FromFile(std::string_view fileName)
	{
		Surface sur{};
		sur.ptr = IMG_Load(fileName.data());
		ARGE_ERROR_HANDLE_NULL(sur.ptr);
		return sur;
	}

	void Surface::Lock()
	{
#ifdef ARGE_DEBUG_MODE
		ARGE_DA(!m_bLocked);
		m_bLocked = true;
#endif
		SDL_LockSurface(ptr);
	}

	void Surface::Unlock()
	{
#ifdef ARGE_DEBUG_MODE
		ARGE_DA(m_bLocked);
		m_bLocked = false;
#endif
		SDL_UnlockSurface(ptr);
	}

	bool Surface::PutPixel(size_t x, size_t y, Color color)
	{
		if (x >= static_cast<size_t>(ptr->w) || y > static_cast<size_t>(ptr->h))
		{
			return false;
		}
		else
		{
			PutPixelUnchecked(x, y, color);
			return true;
		}
	}

	void Surface::PutPixelUnchecked(size_t x, size_t y, Color color)
	{
		ARGE_DA(x < static_cast<size_t>(ptr->w) && y < static_cast<size_t>(ptr->h));
#ifdef ARGE_DEBUG_MODE
		if (!m_bLocked)
		{
			throw ArGenericError{"Tried to modify a surface before locking it"};
		}
#endif

		switch (ptr->format->BytesPerPixel)
		{
		case 3:
		{
			auto const startIndex{3 * (x + y * ptr->w)};
			auto& currRed{static_cast<uint8_t*>(ptr->pixels)[startIndex]};
			auto& currGreen{static_cast<uint8_t*>(ptr->pixels)[startIndex + 1]};
			auto& currBlue{static_cast<uint8_t*>(ptr->pixels)[startIndex + 2]};
			// Good enough...
			currRed = color.r;
			currGreen = color.g;
			currBlue = color.b;
			break;
		}
		case 4:
		{
			auto& currPixel{static_cast<uint32_t*>(ptr->pixels)[x + y * ptr->w]};
			currPixel = color.AsDword(); // This is good enough for now.
			break;
		}
		default:
			throw ArGenericError{"PutPixel is only supported for 32 and 24 bit depths"};
		}
	}

	void Surface::FillRect(size_t x, size_t y, size_t w, size_t h, Color color)
	{
		Rect const rect{
			static_cast<int>(x), static_cast<int>(y), 
			static_cast<int>(w), static_cast<int>(h)
		};
		ARGE_ERROR_HANDLE_NEG(SDL_FillRect(ptr, &rect, color.AsDword()));
	}

	void Surface::FillRect(Rect const& rect, Color color)
	{
		ARGE_ERROR_HANDLE_NEG(SDL_FillRect(ptr, &rect, color.AsDword()));
	}

	void Surface::FillRects(Rect const* rects, size_t count, Color color)
	{
		ARGE_ERROR_HANDLE_NEG(SDL_FillRects(ptr, rects, static_cast<int>(count), color.AsDword()));
	}

	Surface Surface::Doublicate() const
	{
		Surface res{};
		res.ptr = SDL_DuplicateSurface(ptr);
		ARGE_ERROR_HANDLE_NULL(res.ptr);
		return res;
	}

	void Surface::SaveToFile(std::string_view fileName)
	{
		ARGE_ERROR_HANDLE_NEG(SDL_SaveBMP(ptr, fileName.data()));
	}

	bool Surface::HasRLE() const
	{
		return SDL_HasSurfaceRLE(ptr) == SDL_TRUE ? true : false;
	}

	void Surface::SetRLE(bool toWhat)
	{
		ARGE_ERROR_HANDLE_NEG(SDL_SetSurfaceRLE(ptr, toWhat));
	}

	void Surface::SetColorKey(Color color)
	{
		ARGE_ERROR_HANDLE_NEG(SDL_SetColorKey(ptr, 1, color.AsDword()));
	}

	void Surface::DisableColorKey()
	{
		ARGE_ERROR_HANDLE_NEG(SDL_SetColorKey(ptr, 0, 0));
	}

	Color Surface::GetColorKey() const
	{
		uint32_t dword;
		ARGE_ERROR_HANDLE_NEG(SDL_GetColorKey(ptr, &dword));

		return {dword};
	}

	void Surface::SetBlendMode(BlendMode toWhat)
	{
		ARGE_ERROR_HANDLE_NEG(SDL_SetSurfaceBlendMode(ptr, static_cast<SDL_BlendMode>(toWhat)));
	}

	BlendMode Surface::GetBlendMode() const
	{
		SDL_BlendMode mode;
		ARGE_ERROR_HANDLE_NEG(SDL_GetSurfaceBlendMode(ptr, &mode));

		return static_cast<BlendMode>(mode);
	}

	void Surface::SetClipRect(Rect const& rect)
	{
		ARGE_ERROR_HANDLE_NEG(SDL_SetClipRect(ptr, &rect));
	}

	void Surface::DisableClipRect()
	{
		ARGE_ERROR_HANDLE_NEG(SDL_SetClipRect(ptr, nullptr));
	}

	Rect Surface::GetClipRect()
	{
		Rect rect;
		SDL_GetClipRect(ptr, &rect);
		return rect;
	}
}
