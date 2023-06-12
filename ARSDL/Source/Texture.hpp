#pragma once
#include "STD.hpp"
#include "Enums.hpp"
#include "Rect.hpp"

#include <SDL.h>

namespace Arge {
	class Renderer;
	class Surface;

	class Texture
	{
	public:
		Texture(Texture const& rhs)            = default;
		Texture& operator=(Texture const& rhs) = default;
		~Texture();

		constexpr Texture(Texture&& rhs) noexcept : ptr{std::exchange(rhs.ptr, nullptr)}
		{
		}

		constexpr Texture(SDL_Texture* myPtr) noexcept : ptr{myPtr}
		{
		}

		Texture& operator=(Texture&& rhs) noexcept;

		static Texture FromRenderer(Renderer& gfx, TextureAccess access, size_t width, size_t height);
		static Texture FromSurface(Surface& sur, Renderer& gfx);
		static Texture FromFile(std::string_view fileName, Renderer& gfx);

	private:
		Texture() = default;

	public:
		void LockRectForWrite(Rect const& rect, Surface sur);
		void LockRectForWrite(Rect const& rect, void** pixelArr, size_t& pitch);
		void Unlock();

		void SetBlendMode(BlendMode toWhat);
		BlendMode GetBlendMode() const;

		void SetScaleMode(ScaleMode toWhat);
		ScaleMode GetScaleMode() const;

		// Throws for non-static textures.
		void UpdateStatic(Rect const& rect, void const* newPixels, size_t pitch);

	public:
		SDL_Texture* ptr{};
	};
}