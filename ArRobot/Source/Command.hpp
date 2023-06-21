#pragma once
#include "ArRobotCore.hpp"
#include "ArRobotException.hpp"
#include "OpCode.hpp"
#include "Direction.hpp"
#include "BlockType.hpp"

namespace ArRobot {
	// This is an implementation detail.
	// The user (which is only me) will only get to make different types of 
	// commands through the different factory functions inside the Command
	// class, everything else is subject to change.
	enum class CommandType : std::size_t
	{
		DoNothing = 0,

		Move,
		PickUp,
		Drop,

		CheckDir,
		MarkLabel,
		Jump,
		JumpTrue,
		JumpFalse,
		Halt,

		MemSet,
		MemCopy,
		BinaryOp,

		// Debug Commands
		MemPrint,
		MemPrintAll,
	};

	/// Impl block for the CommandType enum.
	namespace Cmd {
		using enum CommandType;

		template <CommandType = static_cast<CommandType>(0I32)>
		struct Data {};

		template <> struct Data<Move>      { std::int32_t x; std::int32_t y; };
		template <> struct Data<CheckDir>  { std::int32_t x; std::int32_t y; BlockType block; };
		template <> struct Data<MarkLabel> { std::string label; };
		template <> struct Data<Jump>      { std::string label; };
		template <> struct Data<JumpTrue>  { std::string label; };
		template <> struct Data<JumpFalse> { std::string label; };
		template <> struct Data<MemSet>    { std::size_t addr; std::int32_t value; };
		template <> struct Data<MemCopy>   { std::size_t toAddr; std::size_t fromAddr; };
		template <> struct Data<BinaryOp>  { OpCode opCode; std::size_t lhsAddr; std::size_t rhsAddr; };
		template <> struct Data<MemPrint>  { std::size_t addr; };

		using Variant = std::variant<
			// Animated:
			Data<DoNothing>, Data<Move>, Data<PickUp>, Data<Drop>, 
			// Control flow:
			Data<CheckDir>, Data<MarkLabel>, Data<Jump>, Data<JumpTrue>, Data<JumpFalse>, 
			Data<Halt>, 
			// Memory:
			Data<MemSet>, Data<MemCopy>, Data<BinaryOp>,
			Data<MemPrint>, Data<MemPrintAll>
		>;
	}

	class Command
	{
	public:
		static constexpr std::size_t InstantPeriod{0};
		static constexpr std::size_t VeryShortPeriod{1};
		static constexpr std::size_t ShortPeriod{2};
		static constexpr std::size_t MediumPeriod{5};
		static constexpr std::size_t LongPeriod{10};
		static constexpr std::size_t VeryLongPeriod{20};

	private:
		constexpr Command(
			CommandType type, Cmd::Variant const& var, std::size_t ticks = VeryShortPeriod) 
			: m_Type{type}, m_Data{var}, m_Ticks{ticks}
		{
		}

	public:
		// This is the best way to do it; any other way I found so far will just make 
		// the code explode in size, for such small performance gain.
		static constexpr Command MakeDoNothing()
		{
			using enum CommandType;
			return {DoNothing, Cmd::Data<DoNothing>{}};
		}

		static constexpr Command MakeMove(std::int32_t delX, std::int32_t delY)
		{
			using enum CommandType;
			return {Move, Cmd::Data<Move>{delX, delY}, LongPeriod};
		}

		static constexpr Command MakePickUp()
		{
			using enum CommandType;
			return {PickUp, Cmd::Data<PickUp>{}, MediumPeriod};
		}

		static constexpr Command MakeDrop()
		{
			using enum CommandType;
			return {Drop, Cmd::Data<Drop>{}, MediumPeriod};
		}

		static constexpr Command MakeCheckDir(std::int32_t x, std::int32_t y, BlockType whatToCheckFor)
		{
			using enum CommandType;
			return {
				CheckDir, 
				Cmd::Data<CheckDir>{x, y, whatToCheckFor},
				ShortPeriod,
			};
		}

		static constexpr Command MakeMarkLabel(std::string_view name)
		{
			using enum CommandType;
			return {MarkLabel, Cmd::Data<MarkLabel>{std::string{name}}, InstantPeriod};
		}

		static constexpr Command MakeJump(std::string_view label)
		{
			using enum CommandType;
			return {Jump, Cmd::Data<Jump>{std::string{label}}, VeryShortPeriod};
		}

		static constexpr Command MakeJumpTrue(std::string_view label)
		{
			using enum CommandType;
			return {JumpTrue, Cmd::Data<JumpTrue>{std::string{label}}, VeryShortPeriod};
		}

		static constexpr Command MakeJumpFalse(std::string_view label)
		{
			using enum CommandType;
			return {JumpFalse, Cmd::Data<JumpFalse>{std::string{label}}, VeryShortPeriod};
		}

		static constexpr Command MakeHalt()
		{
			using enum CommandType;
			return {Halt, Cmd::Data<Halt>{}};
		}

		static constexpr Command MakeMemSet(std::size_t address, std::int32_t value)
		{
			using enum CommandType;
			return {MemSet, Cmd::Data<MemSet>{address, value}, ShortPeriod};
		}

		static constexpr Command MakeMemCopy(std::size_t to, std::size_t from)
		{
			using enum CommandType;
			return {MemCopy, Cmd::Data<MemCopy>{to, from}, ShortPeriod};
		}

		// Stores in lhs.
		static constexpr Command MakeBinaryOp(
			OpCode opCode, std::size_t lhsAddress, std::size_t rhsAddress) 
		{
			using enum CommandType;
			return {
				BinaryOp,
				Cmd::Data<BinaryOp>{
					opCode,
					lhsAddress,
					rhsAddress,
				},
				ShortPeriod,
			};
		}

		static constexpr Command MakeMemPrint(std::size_t address)
		{
			using enum CommandType;
			return {MemPrint, Cmd::Data<MemPrint>{address}, VeryShortPeriod};
		}

		static constexpr Command MakeMemPrintAll()
		{
			using enum CommandType;
			return {MemPrintAll, Cmd::Data<MemPrintAll>{}, VeryShortPeriod};
		}

		std::string ToString() const;
		friend std::ostream& operator<<(std::ostream& lhs, Command const& rhs);

		constexpr CommandType GetType() const
		{
			return m_Type;
		}

		template <CommandType CmdType>
		[[nodiscard]] auto const& As() const
		{
			return std::get<Cmd::Data<CmdType>>(m_Data);
		}

		constexpr std::size_t GetTickCount() const
		{
			return m_Ticks;
		}

	private:
		CommandType m_Type;
		Cmd::Variant m_Data{};
		std::size_t m_Ticks;
	};
}

namespace std {
	template <>
	struct formatter<ArRobot::Command> : formatter<std::string>
	{
		auto format(ArRobot::Command const& cmd, format_context context) const
		{
			return formatter<std::string>{}.format(cmd.ToString(), context);
		}
	};

	template <>
	struct formatter<ArRobot::OpCode> : formatter<std::string_view>
	{
		auto format(ArRobot::OpCode opCode, format_context context) const 
		{
			auto const str{ArRobot::OpCodeEnum::ToString(opCode)};
			return formatter<std::string_view>{}.format(str, context);
		}
	};
}