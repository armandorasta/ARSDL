#pragma once
#include "STD.hpp"

namespace ArRobot {
	enum class Direction : std::int32_t
	{
		North = 1 << 0,
		East  = 1 << 1,
		South = 1 << 2,
		West  = 1 << 3,
	};

	namespace DirectionImpl {
		template <class Operand>
		concept ValidOperand = 
			std::convertible_to<Operand, std::underlying_type_t<Direction>> || 
			std::same_as<Operand, Direction>;
	}

	template <DirectionImpl::ValidOperand Lhs, DirectionImpl::ValidOperand Rhs>
	constexpr std::int32_t operator&(Lhs lhs, Rhs rhs)
	{
		return static_cast<std::int32_t>(lhs) & static_cast<std::int32_t>(rhs);
	}

	template <DirectionImpl::ValidOperand Lhs, DirectionImpl::ValidOperand Rhs>
	constexpr std::int32_t operator|(Lhs lhs, Rhs rhs)
	{
		return static_cast<std::int32_t>(lhs) | static_cast<std::int32_t>(rhs);
	}
}