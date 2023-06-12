#pragma once

#include "STD.hpp"

namespace Arge::Secret {
	template <class Derived, class Value>
	class PointBase
	{
	public:
		constexpr static Derived Clamp(Derived const& vec, Derived const& topLeft, 
			Derived const& botRight)
		{
			return {
				std::clamp(vec.x, topLeft.x, botRight.x),
				std::clamp(vec.y, topLeft.y, botRight.y),
			};
		}

	public:
		constexpr Value Dot(Derived const& rhs) const
		{
			auto& self{CRT()};
			return self.x * rhs.x + self.y * rhs.y;
		}

		constexpr float Mag2() const
		{
			return Dot(CRT());
		}

		constexpr float Mag() const
		{
			return std::sqrt(Mag2());
		}

		constexpr auto Hadamard(Derived const& rhs) const noexcept -> Derived
		{
			auto& self{CRT()};
			return {self.x * rhs.x, self.y * rhs.y};
		}

		Derived CounterClockwiseNormal() const noexcept
		{
			auto& self{CRT()};
			return {self.y, -self.x};
		}

		Derived ClockwiseNormal() const noexcept
		{
			auto& self{CRT()};
			return {-self.y, self.x};
		}

		auto ToString() const -> std::string
		{
			auto& self{CRT()};
			return std::format("({}, {})", self.x, self.y);
		}


	public:
		constexpr auto operator+() const noexcept -> Derived const&
		{
			return CRT();
		}

		constexpr auto operator-() const noexcept -> Derived
		{
			auto& self{CRT()};
			return {-self.x, -self.y};
		}

		constexpr auto operator+(Derived const& rhs) const noexcept -> Derived
		{
			auto& self{CRT()};
			return {self.x + rhs.x, self.y + rhs.y};
		}

		constexpr auto operator-(Derived const& rhs) const noexcept -> Derived
		{
			auto& self{CRT()};
			return {self.x - rhs.x, self.y - rhs.y};
		}

		constexpr auto operator*(Derived const& rhs) const noexcept -> float
		{
			auto& self{CRT()};
			return Dot(rhs);
		}

		constexpr auto operator*(float rhs) const noexcept -> Derived
		{
			auto& self{CRT()};
			return {self.x * rhs, self.y * rhs};
		}

		constexpr friend auto operator*(float lhs, Derived rhs) noexcept -> Derived
		{
			auto& self{CRT()};
			return rhs * lhs;
		}

		constexpr auto operator/(float rhs) const noexcept -> Derived
		{
			auto& self{CRT()};
			return self.operator*(1.f / rhs);
		}


		constexpr Derived& operator+=(Derived const& rhs) noexcept
		{
			auto& self{CRT()};
			return self.operator=(self.operator+(rhs));
		}

		constexpr Derived& operator-=(Derived const& rhs) noexcept
		{
			auto& self{CRT()};
			return self.operator=(self.operator-(rhs));
		}

		constexpr Derived& operator*=(float rhs) noexcept
		{
			auto& self{CRT()};
			return self.operator=(self.operator*(rhs));
		}

		constexpr Derived& operator/=(float rhs) noexcept
		{
			auto& self{CRT()};
			return self.operator=(self.operator/(rhs));
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