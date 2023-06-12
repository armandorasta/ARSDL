#pragma once
#include "PointBase.hpp"

#include <SDL.h>

namespace Arge {
	class Point : public SDL_Point, public Secret::PointBase<Point, int>
	{
	public:
		constexpr Point() : SDL_Point{} { }
		constexpr Point(int x, int y) : SDL_Point{x, y} { }

		constexpr std::partial_ordering operator<=>(Point const& rhs) const = default;
	};

	using Vec2i = Point;

}

namespace std {
	template <>
	struct formatter<Arge::Point> : public formatter<std::string>
	{
		auto format(Arge::Point const& vec, format_context context)
		{
			return vformat_to(context.out(), vec.ToString(), std::make_format_args());
		}
	};
}