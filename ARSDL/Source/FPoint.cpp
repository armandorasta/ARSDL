#include "pch.h"
#include "FPoint.hpp"

namespace ArSDL {
	float FPoint::Dist(CRef lhs, CRef rhs)
	{
		return std::sqrt(Dist2(lhs, rhs));
	}

	auto FPoint::Normalized() const noexcept -> Self
	{
		auto const mag{Mag()};
		return Util::FloatEq(mag, 0.f) ? FPoint{} : operator/(mag);
	}

	void FPoint::Normalize() noexcept
	{
		operator=(Normalized());
	}

	auto FPoint::Rotated(float angle) const noexcept -> Self
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

	void FPoint::Rotate(float angle) noexcept
	{
		operator=(Rotated(angle));
	}
}