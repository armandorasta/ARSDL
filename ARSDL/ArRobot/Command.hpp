#pragma once
#include "STD.hpp"

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

	enum class CommandType : std::size_t
	{
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
	};

	struct Command
	{
		static constexpr Command MakeMove(std::int32_t delX, std::int32_t delY)
		{
			return {CommandType::Move, {delX, delY}};
		}

		static constexpr Command MakePickUp()
		{
			return {CommandType::PickUp};
		}

		static constexpr Command MakeDrop()
		{
			return {CommandType::Drop};
		}

		static constexpr Command MakeJump(std::int32_t lineNumber)
		{
			return {CommandType::Jump, {lineNumber}};
		}

		static constexpr Command MakeJumpTrue(std::int32_t lineNumber)
		{
			return {CommandType::JumpTrue, {lineNumber}};
		}

		static constexpr Command MakeJumpFalse(std::int32_t lineNumber)
		{
			return {CommandType::JumpFalse, {lineNumber}};
		}

		static constexpr Command MakeMemSet(std::size_t address, std::int32_t value)
		{
			return {CommandType::MemSet, {static_cast<std::int32_t>(address), value}};
		}

		static constexpr Command MakeMemCopy(std::size_t to, std::size_t from)
		{
			return {
				CommandType::MemCopy,
				{static_cast<std::int32_t>(to), static_cast<std::int32_t>(from)}
			};
		}

		static constexpr Command MakeIncrement(std::size_t address, std::int32_t incValue)
		{
			return {
				CommandType::Increment,
				{static_cast<std::int32_t>(address), incValue}
			};
		}

		// Stores in lhs.
		static constexpr Command MakeBinaryOp(OpCode opCode, std::size_t lhsAddress,
			std::size_t rhsAddress)
		{
			return {
				CommandType::BinaryOp,
				std::array{
					static_cast<std::int32_t>(opCode),
					static_cast<std::int32_t>(lhsAddress),
					static_cast<std::int32_t>(rhsAddress),
				}
			};
		}

		CommandType type;
		std::array<std::int32_t, 3> data{};
	};
}