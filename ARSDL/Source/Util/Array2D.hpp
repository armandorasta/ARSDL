#pragma once
#include "STD.hpp"
#include "ArSDLError.hpp"

namespace ArSDL {
	template <class TValue, size_t Width, size_t Height>
	class Array2D
	{
	private:
		using Self = Array2D;

	public:
		constexpr TValue& At(size_t x, size_t y)
		{
			ARSDL_DA(x < Width);
			ARSDL_DA(y < Height);
			return m_Data[x + y * Width];
		}

		constexpr TValue const& At(size_t x, size_t y) const
		{
			return const_cast<Self&>(*this).At(x, y);
		}

		constexpr TValue& AtIndex(size_t index)
		{
			ARSDL_DA(index < GetSize());
			return m_Data[index];
		}

		constexpr TValue const& AtIndex(size_t index) const
		{
			return const_cast<Self&>(*this).AtIndex(index);
		}

		constexpr TValue const* GetDataPtr() const
		{
			return m_Data.data();
		}

		constexpr TValue* GetDataPtr()
		{
			return m_Data.data();
		}

		constexpr size_t GetSize() const
		{
			return m_Data.size();
		}

		constexpr size_t GetWidth() const
		{
			return Width;
		}

		constexpr size_t GetHeight() const
		{
			return Height;
		}

		// No c or r versions for now.
		constexpr auto begin() const { return m_Data.begin(); }
		constexpr auto end()   const { return m_Data.end(); }
		constexpr auto begin() { return m_Data.begin(); }
		constexpr auto end()   { return m_Data.end(); }

	private:
		std::array<TValue, Width * Height> m_Data{};
	};
}