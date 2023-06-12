#pragma once
#include "STD.hpp"
#include "ArRobotException.hpp"

/*
	Global constants {
		Directions {
			_West,
			_NorthWest,
			_North,
			_NorthEast,
			...
		}

		Block types {
			_Wall,
			_Item,
			_Floor,
			...
		}
	}

	Built-in functions {
		Fundamental {
			_Move(Direction),
			_Take(Direction),
			_Drop(Direction),
		}

		Utility {
			_Contain_same,
			_Are_same(
		}
	}

	Keywords {
		if, else, elif,
		while, for
		func, var,
		contains,

	}

	while _Left is _Wall && _Right is _Wall {
		move(_North)
	}

	while _Are_same(_Left, _Right) {
		move(_North)
	}

	while _All_are(_Wall, _Left, _Right) {
		move(_North)
	}

	for_all as dir {

	}
*/

namespace ArRobot {
	enum class OpCode : std::int32_t
	{
		Add = 1,
		Sub,
		Mul,
		Div,

		Equal,
		NotEqual,
		Greater,
		GreaterEq,
		Less,
		LessEq,
	};

	constexpr std::string_view OpCodeToString(OpCode opCode)
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
		default:        return "Invalid";
		}
	}

	constexpr std::int32_t OpCodeEval(OpCode opCode, std::int32_t lhs, std::int32_t rhs)
	{
		using enum OpCode;
		switch (opCode)
		{
		case Add:       return lhs +  rhs;
		case Sub:		return lhs -  rhs;
		case Mul:		return lhs *  rhs;
		case Div:		return lhs /  rhs;
		case Equal:		return lhs == rhs ? 1 : 0;
		case NotEqual:	return lhs != rhs ? 1 : 0;
		case Greater:	return lhs >  rhs ? 1 : 0;
		case GreaterEq:	return lhs >= rhs ? 1 : 0;
		case Less:		return lhs <  rhs ? 1 : 0;
		case LessEq:	return lhs <= rhs ? 1 : 0;
		default:
			throw GenericError{"Evaluating invalid OpCode: {}", static_cast<std::int32_t>(opCode)};
		}
	}

	enum class CommandType : std::size_t
	{
		NoOp = 0,

		Move,
		PickUp,
		Drop,

		Jump,
		JumpTrue,
		JumpFalse,

		MemSet,
		MemCopy,
		Increment,

		BinaryOp,

		// Debug Commands
		MemPrint,
		MemPrintAll,
	};

	struct Command
	{
		static constexpr std::size_t VeryShortPeriod{1};
		static constexpr std::size_t ShortPeriod{2};
		static constexpr std::size_t MediumPeriod{5};
		static constexpr std::size_t LongPeriod{10};
		static constexpr std::size_t VeryLongPeriod{20};

		static constexpr Command MakeNoOp()
		{
			return {CommandType::NoOp};
		}

		static constexpr Command MakeMove(std::int32_t delX, std::int32_t delY)
		{
			return {CommandType::Move, {delX, delY}, VeryLongPeriod};
		}

		static constexpr Command MakePickUp()
		{
			return {CommandType::PickUp, {}, MediumPeriod};
		}

		static constexpr Command MakeDrop()
		{
			return {CommandType::Drop, {}, MediumPeriod};
		}

		static constexpr Command MakeJump(std::int32_t lineNumber)
		{
			return {CommandType::Jump, {lineNumber}, VeryShortPeriod};
		}

		static constexpr Command MakeJumpTrue(std::int32_t lineNumber)
		{
			return {CommandType::JumpTrue, {lineNumber}, VeryShortPeriod};
		}

		static constexpr Command MakeJumpFalse(std::int32_t lineNumber)
		{
			return {CommandType::JumpFalse, {lineNumber}, VeryShortPeriod};
		}

		static constexpr Command MakeMemSet(std::size_t address, std::int32_t value)
		{
			return {CommandType::MemSet, {static_cast<std::int32_t>(address), value}, ShortPeriod};
		}

		static constexpr Command MakeMemCopy(std::size_t to, std::size_t from)
		{
			return {
				CommandType::MemCopy,
				{static_cast<std::int32_t>(to), static_cast<std::int32_t>(from)},
				ShortPeriod,
			};
		}

		static constexpr Command MakeIncrement(std::size_t address, std::int32_t incValue)
		{
			return {
				CommandType::Increment,
				{static_cast<std::int32_t>(address), incValue},
				ShortPeriod,
			};
		}

		// Stores in lhs.
		static constexpr Command MakeBinaryOp(
			OpCode opCode, std::size_t lhsAddress, std::size_t rhsAddress)
		{
			return {
				CommandType::BinaryOp,
				std::array{
					static_cast<std::int32_t>(opCode),
					static_cast<std::int32_t>(lhsAddress),
					static_cast<std::int32_t>(rhsAddress),
				},
				ShortPeriod,
			};
		}

		static constexpr Command MakeMemPrint(std::size_t address)
		{
			return {
				CommandType::MemPrint,
				{ static_cast<std::int32_t>(address) },
				VeryShortPeriod,
			};
		}

		static constexpr Command MakeMemPrintAll()
		{
			return {CommandType::MemPrintAll, {}, VeryShortPeriod};
		}

		std::string ToString() const;
		friend std::ostream& operator<<(std::ostream& lhs, Command const& rhs);

		CommandType type;
		std::array<std::int32_t, 3> data{};
		std::size_t ticks;
	};
}

namespace std {
	template <>
	struct formatter<ArRobot::Command> : std::formatter<std::string>
	{
		auto format(ArRobot::Command const& cmd, format_context context)
		{
			return vformat_to(context.out(), cmd.ToString(), std::make_format_args());
		}
	};

	template <>
	struct formatter<ArRobot::OpCode> : std::formatter<std::string>
	{
		auto format(ArRobot::OpCode const& opCode, format_context context)
		{
			return vformat_to(context.out(), OpCodeToString(opCode), std::make_format_args());
		}
	};
}