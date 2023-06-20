#pragma once
#include "ArRobotException.hpp"

namespace ArRobot {
	namespace OpCodeEnum {
		constexpr auto UOpBit{1I32 << 31};
		constexpr auto BiOpBit{1I32 << 30};
	}

	enum class OpCode : std::int32_t
	{
		Add       = OpCodeEnum::BiOpBit | 1,
		Sub       = OpCodeEnum::BiOpBit | 2,
		Mul       = OpCodeEnum::BiOpBit | 3,
		Div       = OpCodeEnum::BiOpBit | 4,

		Equal     = OpCodeEnum::BiOpBit | 5,
		NotEqual  = OpCodeEnum::BiOpBit | 6,
		Greater   = OpCodeEnum::BiOpBit | 7,
		GreaterEq = OpCodeEnum::BiOpBit | 8,
		Less      = OpCodeEnum::BiOpBit | 9,
		LessEq    = OpCodeEnum::BiOpBit | 10,

		And       = OpCodeEnum::BiOpBit | 11,
		Or        = OpCodeEnum::BiOpBit | 12, 
		Xor       = OpCodeEnum::BiOpBit | 13,

		BitAnd    = OpCodeEnum::BiOpBit | 14,
		BitOr     = OpCodeEnum::BiOpBit | 15,
		BitXor    = OpCodeEnum::BiOpBit | 16,

		Not       = OpCodeEnum::UOpBit | 1,
		BitNot    = OpCodeEnum::UOpBit | 2,
	};

	namespace OpCodeEnum {
		constexpr std::string_view ToString(OpCode opCode)
		{
			using enum OpCode;
			switch (opCode)
			{
			case Add:       return "Add";
			case Sub:       return "Sub";
			case Mul:       return "Mul";
			case Div:       return "Div";
			case Equal:     return "Equal";
			case NotEqual:  return "NotEqual";
			case Greater:   return "Greater";
			case GreaterEq: return "GreaterEq";
			case Less:      return "Less";
			case LessEq:    return "LessEq";
			case And:       return "And";
			case Or:        return "Or";
			case Xor:       return "Xor";
			case BitAnd:    return "BitAnd";
			case BitOr:     return "BitOr";
			case BitXor:    return "BitXor";
			case Not:       return "Not";
			case BitNot:    return "BitNot";
			default:        return "Invalid";
			}
		}

		constexpr std::int32_t Eval(OpCode opCode, std::int32_t lhs, std::int32_t rhs)
		{
			using enum OpCode;
			switch (opCode)
			{
			case Add:       return lhs +  rhs;
			case Sub:       return lhs -  rhs;
			case Mul:       return lhs *  rhs;
			case Div:       return lhs /  rhs;
			case Equal:     return lhs == rhs ? 1 : 0;
			case NotEqual:  return lhs != rhs ? 1 : 0;
			case Greater:   return lhs >  rhs ? 1 : 0;
			case GreaterEq: return lhs >= rhs ? 1 : 0;
			case Less:      return lhs <  rhs ? 1 : 0;
			case LessEq:    return lhs <= rhs ? 1 : 0;
			case And:       return lhs && rhs ? 1 : 0;
			case Or:		return lhs || rhs ? 1 : 0;
			case Xor:		return (lhs != 0) != (rhs != 0) ? 1 : 0;
			case BitAnd:	return lhs &  rhs;
			case BitOr:		return lhs |  rhs;
			case BitXor:	return lhs ^  rhs;
			case Not:		return lhs == 0;
			case BitNot:	return ~lhs;
			default:
				throw GenericError{"Evaluating invalid OpCode: {}", static_cast<std::int32_t>(opCode)};
			}
		}
	}
}