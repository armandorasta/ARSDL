#include "pch.h"
#include "Texture.hpp"
#include "Renderer.hpp"
#include "ArSDLError.hpp"
#include "Surface.hpp"

#include <SDL/SDL_image.h>

namespace Arge {
	Texture& Texture::operator=(Texture&& rhs) noexcept
	{
		if (&rhs != this)
		{
			SDL_DestroyTexture(ptr);
			ptr = std::exchange(rhs.ptr, nullptr);
		}
		return *this;
	}

	Texture::~Texture()
	{
		SDL_DestroyTexture(ptr);
	}

	Texture Texture::FromRenderer(Renderer& gfx, TextureAccess access, size_t width, size_t height)
	{
		Texture res{};
		res.ptr = SDL_CreateTexture(gfx.ptr, SDL_PixelFormatEnum::SDL_PIXELFORMAT_BGRA8888,
			static_cast<int>(access), static_cast<int>(width), static_cast<int>(height));
		ARGE_ERROR_HANDLE_NULL(res.ptr);
		return res;
	}

	Texture Texture::FromSurface(Surface& sur, Renderer& gfx)
	{
		Texture res{};
		res.ptr = SDL_CreateTextureFromSurface(gfx.ptr, sur.ptr);
		ARGE_ERROR_HANDLE_NULL(res.ptr);
		return res;
	}

	Texture Texture::FromFile(std::string_view fileName, Renderer& gfx)
	{
		Texture res{};
		res.ptr = IMG_LoadTexture(gfx.ptr, fileName.data());
		ARGE_ERROR_HANDLE_NULL(res.ptr);
		return res;
	}

	void Texture::LockRectForWrite(Rect const& rect, Surface sur)
	{
		ARGE_ERROR_HANDLE_NEG(SDL_LockTextureToSurface(ptr, &rect, &sur.ptr));
	}

	void Texture::LockRectForWrite(Rect const& rect, void** pixelArr, size_t& pitch)
	{
		int intPitch{};
		SDL_LockTexture(ptr, &rect, pixelArr, &intPitch);
		pitch = static_cast<size_t>(intPitch);
	}

	void Texture::Unlock()
	{
		SDL_UnlockTexture(ptr);
	}

	void Texture::SetBlendMode(BlendMode toWhat)
	{
		ARGE_ERROR_HANDLE_NEG(SDL_SetTextureBlendMode(ptr, static_cast<SDL_BlendMode>(toWhat)));
	}

	BlendMode Texture::GetBlendMode() const
	{
		SDL_BlendMode mode{};
		ARGE_ERROR_HANDLE_NEG(SDL_GetTextureBlendMode(ptr, &mode));
		return static_cast<BlendMode>(mode);
	}

	void Texture::SetScaleMode(ScaleMode toWhat)
	{
		ARGE_ERROR_HANDLE_NEG(SDL_SetTextureScaleMode(ptr, static_cast<SDL_ScaleMode>(toWhat)));
	}

	ScaleMode Texture::GetScaleMode() const
	{
		SDL_ScaleMode mode{};
		ARGE_ERROR_HANDLE_NEG(SDL_GetTextureScaleMode(ptr, &mode));
		return static_cast<ScaleMode>(mode);
	}

	void Texture::UpdateStatic(Rect const& rect, void const* newPixels, size_t pitch)
	{
		SDL_UpdateTexture(ptr, &rect, newPixels, static_cast<int>(pitch));
	}
}
