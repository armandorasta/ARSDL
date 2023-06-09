#pragma once
#include "STD.hpp"
#include "Array2D.hpp"

namespace ArSDL {
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

		constexpr float GetCellWidth() const
		{
			return m_CellWidth;
		}

		constexpr float GetCellHeight() const
		{
			return m_CellHeight;
		}

		constexpr std::pair<size_t, size_t> ScreenToGrid(FPoint const& point)
		{
			return {
				static_cast<size_t>(point.x / m_CellWidth),
				static_cast<size_t>(point.y / m_CellHeight),
			};
		}

		constexpr FPoint GridToScreen(size_t x, size_t y)
		{
			return {x * m_CellWidth, y * m_CellHeight};
		}

	private:
		float m_CellWidth;
		float m_CellHeight;
	};
}