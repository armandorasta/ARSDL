#pragma once
#include "STD.hpp"
#include "Array2D.hpp"

namespace ArSDL {
	/// This is the exact same as Array2D, except it keeps track of the size of the cell,
	/// and also provides transformation functions between screen-space and grid-index-space.
	template <class ValueType, size_t Width, size_t Height>
	class Grid : public Array2D<ValueType, Width, Height>
	{
	private:
		using Self = Grid;
		using Base = Array2D<ValueType, Width, Height>;

	public:
		Grid(Self const&)                = default;
		Grid(Self&&) noexcept            = default;
		Grid& operator=(Self const&)     = default;
		Grid& operator=(Self&&) noexcept = default;

		constexpr Grid(float cellWidth, float cellHeight)
			: m_CellWidth{cellWidth}, m_CellHeight{cellHeight}
		{
		}

		[[nodiscard]]
		constexpr float GetCellWidth() const
		{
			return m_CellWidth;
		}

		[[nodiscard]]
		constexpr float GetCellHeight() const
		{
			return m_CellHeight;
		}

		constexpr void SetCellWidth(float newWidth)
		{
			m_CellWidth = newWidth;
		}

		constexpr void SetCellHeight(float newHeight)
		{
			m_CellHeight = newHeight;
		}

		[[nodiscard]]
		constexpr std::pair<size_t, size_t> ScreenToGrid(FPoint const& point) const
		{
			return {
				static_cast<size_t>(point.x / m_CellWidth),
				static_cast<size_t>(point.y / m_CellHeight),
			};
		}

		[[nodiscard]]
		constexpr FPoint GridToScreen(size_t x, size_t y) const
		{
			return {x * m_CellWidth, y * m_CellHeight};
		}

		[[nodiscard]]
		constexpr bool operator==(Self const& rhs) const
		{
			return Base::operator==(rhs);
		}

	private:
		float m_CellWidth;
		float m_CellHeight;
	};
}