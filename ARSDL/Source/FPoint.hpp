#pragma once

#include "PointBase.hpp"
#include "Util.hpp"

namespace ArSDL {
	class FPoint : public SDL_FPoint, public Secret::PointBase<FPoint, float>
	{
	public:
		using Self = FPoint;
		using CRef = FPoint const&;

		constexpr FPoint(Self const&)          = default;
		constexpr FPoint(Self&&)               = default;
		constexpr Self& operator=(Self const&) = default;
		constexpr Self& operator=(Self&&)      = default;

		constexpr FPoint() : SDL_FPoint{} { }
		constexpr FPoint(float x, float y) : SDL_FPoint{x, y} { }

		/** 
			* @brief creats a vector using a direction and a length.
			* @param dir => the direction of the vector (angle in radians)
			* @param len => the length of the vector (defaults to 1)
			* @return <cos(dir), sin(dir)> * len
			*/
		constexpr static Self FromDir(float dir, float len = 1.f)
		{
			return Self{std::cos(dir), std::sin(dir)} * len;
		}

		/**
			* @return the square distance between two vectors.
		*/
		constexpr static float Dist2(CRef lhs, CRef rhs)
		{
			return (lhs - rhs).Mag2();
		}

		/**
			* @return the distance between two vectors.
		*/
		static float Dist(CRef lhs, CRef rhs);

		/**
			* @return rhs - lhs.
		*/
		constexpr static Self Subtract(CRef lhs, CRef rhs)
		{ return lhs - rhs; }

		/**
			* @return rhs + lhs.
		*/
		constexpr static Self Add(CRef lhs, CRef rhs)
		{ return lhs + rhs; }

	public:

		/**
			* @return the length of the resulting vector of the cross product between the current vector and rhs.
		*/
		constexpr auto Cross(CRef rhs) const noexcept -> float
		{ return x * rhs.y - rhs.x * y; }

		/**
			* @return a normlized version of the current vector (non-mutating).
		*/
		auto Normalized() const noexcept ->Self;
		
		/**
			* @brief normalizes the current vector (mutating).
		*/
		void Normalize() noexcept;

		/**
			* @param angle => the angle (in rad) to rotate the vector by.
			* @return a rotated version of the current vector (non-mutating).
		*/
		auto Rotated(float angle) const noexcept -> Self;

		/**
			* @brief rotates the vector inplace (mutating).
			* @param angle => the angle (in rad) to rotate the vector by.
		*/
		void Rotate(float angle) noexcept;

		/**
			* @brief does a better job than == at checking this.
			* @param rhs => the vector to be compared with.
			* @return true if the two vectors are close enough to be considered equal,
			* false otherwise.
		*/
		constexpr auto Equals(CRef rhs) const noexcept -> bool
		{
			return Util::FloatEq(x, rhs.x) and Util::FloatEq(y, rhs.y);
		}

		/**
			* @brief this is more raliable at checking equality than <=>.
			* @param rhs => the vector to be compared with.
			* @return the order (strong unlike the built in) of two vectors.
		*/
		constexpr auto Compare(CRef rhs) const noexcept -> std::strong_ordering
		{
			constexpr auto compareFloats = [](float lhs, float rhs) constexpr {
				if (lhs > rhs) return std::strong_ordering::greater;
				if (lhs < rhs) return std::strong_ordering::less;
				return std::strong_ordering::equal;
			};

			if (Util::FloatEq(x, rhs.x))
			{
				return compareFloats(y, rhs.y);
			}
			else
			{
				return compareFloats(x, rhs.x);
			}
		}

		constexpr std::partial_ordering operator<=>(CRef rhs) const = default;

	public:
		const static Self Zero;
		const static Self UnitX;
		const static Self UnitY;
		const static Self One;
	};

	inline const FPoint::Self FPoint::Zero{0.f, 0.f};
	inline const FPoint::Self FPoint::UnitX{1.f, 0.f};
	inline const FPoint::Self FPoint::UnitY{0.f, 1.f};
	inline const FPoint::Self FPoint::One{1.f, 1.f};

	using Vec2 = FPoint;
}

namespace std {
	template <>
	struct formatter<ArSDL::FPoint> : public formatter<std::string> 
	{
		auto format(ArSDL::FPoint const& vec, format_context context) 
		{ return vformat_to(context.out(), vec.ToString(), std::make_format_args()); }
	};
}