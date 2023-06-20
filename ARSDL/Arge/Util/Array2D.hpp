#pragma once
#include "ArgeCore.hpp"
#include "ArSDLError.hpp"

namespace Arge {
	template <class TValue>
	class Array2D
	{
	private:
		using Self = Array2D;

	public:
		Array2D(size_t width, size_t height) : m_Width{width}, m_Height{height}
		{
			m_Data.resize(width * height);
		}

		[[nodiscard]]
		constexpr bool operator==(Self const& rhs) const = default;
		
		[[nodiscard]]
		constexpr TValue& At(size_t x, size_t y)
		{
			ARSDL_DA(x < m_Width);
			ARSDL_DA(y < m_Height);
			return m_Data[x + y * m_Width];
		}

		[[nodiscard]]
		constexpr TValue const& At(size_t x, size_t y) const
		{
			return const_cast<Self&>(*this).At(x, y);
		}

		[[nodiscard]]
		constexpr TValue& AtIndex(size_t index)
		{
			ARSDL_DA(index < GetSize());
			return m_Data[index];
		}

		[[nodiscard]]
		constexpr TValue const& AtIndex(size_t index) const
		{
			return const_cast<Self&>(*this).AtIndex(index);
		}

		[[nodiscard]]
		constexpr TValue const* GetDataPtr() const
		{
			return m_Data.data();
		}

		[[nodiscard]]
		constexpr TValue* GetDataPtr()
		{
			return m_Data.data();
		}

		[[nodiscard]]
		constexpr size_t GetSize() const
		{
			return m_Data.size();
		}

		[[nodiscard]]
		constexpr size_t GetWidth() const
		{
			return m_Width;
		}

		[[nodiscard]]
		constexpr size_t GetHeight() const
		{
			return m_Height;
		}

		[[nodiscard]]
		constexpr bool IsInBounds(std::size_t x, std::size_t y) const
		{
			return x < m_Width && y < m_Height;
		}

		[[nodiscard]]
		constexpr bool IsInBoundsSigned(std::int32_t x, std::int32_t y) const
		{
			return 0 <= x  && x < m_Width && 0 <= y && y < m_Height;
		}

		// No c or r versions for now.
		constexpr auto begin() const { return m_Data.begin(); }
		constexpr auto end()   const { return m_Data.end(); }
		constexpr auto begin() { return m_Data.begin(); }
		constexpr auto end()   { return m_Data.end(); }

	private:
		size_t m_Width;
		size_t m_Height;
		std::vector<TValue> m_Data{};
	};
}