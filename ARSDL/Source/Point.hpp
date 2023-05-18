#pragma once
#include "PointBase.hpp"

#include <SDL.h>

namespace ArSDL {
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
	struct formatter<ArSDL::Point> : public formatter<std::string>
	{
		auto format(ArSDL::Point const& vec, format_context context)
		{
			return vformat_to(context.out(), vec.ToString(), std::make_format_args());
		}
	};
}