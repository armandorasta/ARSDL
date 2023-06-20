#include "pch.h"
#include "Vec2.hpp"

namespace Arge {
	float Vec2::Dist(CRef lhs, CRef rhs)
	{
		return std::sqrt(Dist2(lhs, rhs));
	}

	auto Vec2::Normalized() const noexcept -> Self
	{
		auto const mag{Mag()};
		return Util::FloatEq(mag, 0.f) ? Vec2{} : operator/(mag);
	}

	void Vec2::Normalize() noexcept
	{
		operator=(Normalized());
	}

	auto Vec2::Rotated(float angle) const noexcept -> Self
	{
		// did not use SinCos because I am planning to switch to 
		// std::sin and std::cos in c++23
		float fCos{std::cos(angle)};
		float fSin{std::sin(angle)};
		return {
			x * fCos - y * fSin,
			x * fSin + y * fCos,
		};
	}

	void Vec2::Rotate(float angle) noexcept
	{
		operator=(Rotated(angle));
	}
}