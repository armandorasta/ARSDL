#pragma once
#include "STD.hpp"
#include "Vec2.hpp"
#include "Point.hpp"

namespace Arge {
	namespace Secret {
		template <class Type, class Selection0, class Selection1>
		concept OneOf = std::same_as<Type, Selection0> || std::same_as<Type, Selection1>;

		template <class Element>
		using PointFromEl_t = std::conditional_t<std::is_same_v<int, Element>, Point, Vec2>;
	}

	class RectF;
	class Rect;

	template <class Derived, class Value>
	class RectBase
	{
	public:
		constexpr Value GetArea() const
		{
			auto& _this{CRT()};
			return _this.w * _this.h;
		}

		constexpr Value GetPerimeter() const
		{
			auto& _this{CRT()};
			return 2 * (_this.w + _this.h);
		}

		template <Secret::OneOf<int, float> Element = float>
		constexpr Secret::PointFromEl_t<Element> GetCenter() const
		{
			auto& _this{CRT()};
			return {
				static_cast<Element>(_this.x + _this.w * 0.5f),
				static_cast<Element>(_this.y + _this.h * 0.5f),
			};
		}

		template <Secret::OneOf<Vec2, Point> Contained>
		constexpr bool Contains(Contained const& that) const 
		{
			auto& _this{CRT()};
			return _this.x < that.x && that.x < _this.x + _this.w
				&& _this.y < that.y && that.y < _this.y + _this.h;
		}

		template <Secret::OneOf<RectF, Rect> Contained>
		constexpr bool Contains(Contained const& that) const
		{
			auto& _this{CRT()};
			/*
				*==============*
				| *====*       |
				| |    |       |
				| *====*       |
				*==============*
			*/
			return _this.x < that.x && _this.x + _this.w > that.x + that.w
				&& _this.y < that.y && _this.y + _this.h > that.y + that.h;
		}

		template <Secret::OneOf<RectF, Rect> RectType>
		constexpr bool OverlapsWith(RectType const& that) const
		{
			auto& _this{CRT()};
			/*
				*==============*
				|         *====|=========*
				|         |    |         |
				*=========|====*         |
				          *==============*
			*/
			return _this.x < that.x + that.w && _this.x + _this.w > that.x
				&& _this.y < that.y + that.h && _this.y + _this.h > that.y;
		}

		template <Secret::OneOf<RectF, Rect> RectType, Secret::OneOf<int, float> Element = float>
		constexpr Secret::PointFromEl_t<Element> CalcOverlap(RectType const& that) const
		{
			using PointType = Secret::PointFromEl_t<Element>;

			auto& _this{CRT()};
			PointType const del0{
				(that.x + that.w) - _this.x,
				(that.y + that.h) - _this.y,
			};
			PointType const del1{
				that.x - (_this.x + _this.w),
				that.y - (_this.y + _this.h),
			};
			auto const xDel{std::abs(del0.x) < std::abs(del1.x) ? del0.x : del1.x};
			auto const yDel{std::abs(del0.y) < std::abs(del1.y) ? del0.y : del1.y};

			// Move out with the least amount of distance possible.
			// If greater than or equal which (hopefully) will pioritize the x-axis.
			return std::abs(yDel) < std::abs(xDel) ? PointType{0, yDel} : PointType{xDel, 0};
		}

		template <Secret::OneOf<RectF, Rect> RectType>
		constexpr void PushOutOf(RectType const& that)
		{
			auto& _this{CRT()};
			if (&_this != &that && OverlapsWith(that))
			{
				auto const overlap{_this.CalcOverlap(that)};
				_this.x += overlap.x;
				_this.y += overlap.y;
			}
		}

		template <Secret::OneOf<RectF, Rect> RectType>
		constexpr bool Intersects(RectType const& that) const noexcept
		{
			auto& _this{CRT()};
			return _this.OverlapsWith(that) && !Contains(that) && !that.Contains(_this);
		}

	private:
		constexpr Derived& CRT()
		{
			return static_cast<Derived&>(*this);
		}

		constexpr Derived const& CRT() const
		{
			return static_cast<Derived const&>(*this);
		}
	};
}