#pragma once
#include "STD.hpp"
#include "ArRobotException.hpp"
#include "OpCode.hpp"
#include "Direction.hpp"
#include "BlockType.hpp"
#include "Point.hpp"

namespace ArRobot {
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
		Increment,

		BinaryOp,

		// Debug Commands
		MemPrint,
		MemPrintAll,
	};

	class Command
	{
	private:
		using ToStringFunc = std::function<std::string(Command const&)>;
		inline static std::unordered_map<CommandType, ToStringFunc> s_ToStringFuncMap{};

		struct StringDataTagType { };
		static constexpr StringDataTagType StringDataTag{};

	public:
		static constexpr std::size_t InstantPeriod{1};
		static constexpr std::size_t VeryShortPeriod{1};
		static constexpr std::size_t ShortPeriod{2};
		static constexpr std::size_t MediumPeriod{5};
		static constexpr std::size_t LongPeriod{10};
		static constexpr std::size_t VeryLongPeriod{20};

		constexpr Command(CommandType type, std::array<std::int32_t, 3> const& data = {},
			std::size_t ticks = VeryShortPeriod) :
			m_Type{type}, m_IntData{data}, m_Ticks{ticks}
		{
		}

		constexpr Command(StringDataTagType, CommandType type, std::string_view data, 
			std::size_t ticks = VeryShortPeriod) : 
			m_Type{type}, m_StringData{std::string{data}}, m_Ticks{ticks}
		{
		}

		static constexpr Command MakeDoNothing()
		{
			return {CommandType::DoNothing};
		}

		static constexpr Command MakeMove(std::int32_t delX, std::int32_t delY)
		{
			return {CommandType::Move, { delX, delY }, LongPeriod};
		}

		static constexpr Command MakePickUp()
		{
			return {CommandType::PickUp, { }, MediumPeriod};
		}

		static constexpr Command MakeDrop()
		{
			return {CommandType::Drop, { }, MediumPeriod};
		}

		static constexpr Command MakeCheckDir(std::int32_t x, std::int32_t y, BlockType whatToCheckFor)
		{
			return {
				CommandType::CheckDir, 
				{ x, y, static_cast<std::int32_t>(whatToCheckFor) },
				ShortPeriod,
			};
		}

		static constexpr Command MakeMarkLabel(std::string_view name)
		{
			return {StringDataTag, CommandType::MarkLabel, name, InstantPeriod};
		}

		static constexpr Command MakeJump(std::string_view label)
		{
			return {StringDataTag, CommandType::Jump, label, VeryShortPeriod};
		}

		static constexpr Command MakeJumpTrue(std::string_view label)
		{
			return {StringDataTag, CommandType::JumpTrue, label, VeryShortPeriod};
		}

		static constexpr Command MakeJumpFalse(std::string_view label)
		{
			return {StringDataTag, CommandType::JumpFalse, label, VeryShortPeriod};
		}

		static constexpr Command MakeHalt()
		{
			return {CommandType::Halt};
		}

		static constexpr Command MakeMemSet(std::size_t address, std::int32_t value)
		{
			return {CommandType::MemSet, { static_cast<std::int32_t>(address), value }, ShortPeriod};
		}

		static constexpr Command MakeMemCopy(std::size_t to, std::size_t from)
		{
			return {
				CommandType::MemCopy,
				{ static_cast<std::int32_t>(to), static_cast<std::int32_t>(from) },
				ShortPeriod,
			};
		}

		static constexpr Command MakeIncrement(std::size_t address, std::int32_t incValue)
		{
			return {
				CommandType::Increment,
				{ static_cast<std::int32_t>(address), incValue },
				ShortPeriod,
			};
		}

		// Stores in lhs.
		static constexpr Command MakeBinaryOp(OpCode opCode, std::size_t lhsAddress, 
			std::size_t rhsAddress)
		{
			return {
				CommandType::BinaryOp,
				{
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
			return {CommandType::MemPrintAll, { }, VeryShortPeriod};
		}

		std::string ToString() const;
		friend std::ostream& operator<<(std::ostream& lhs, Command const& rhs);

		constexpr CommandType GetType() const
		{
			return m_Type;
		}

		template <CommandType CmdType>
		[[nodiscard]] auto As() const;

		constexpr std::size_t GetTickCount() const
		{
			return m_Ticks;
		}

	private:
		CommandType m_Type;
		// Still using an array because it's the same size as a vector, but much faster.
		std::array<std::int32_t, 3> m_IntData{};
		// No using std::variant because future commands may have both types of data.
		// Also std::variant takes extra space anyway.
		std::string m_StringData{};
		std::size_t m_Ticks;
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
			return vformat_to(context.out(), ArRobot::OpCodeEnum::ToString(opCode), 
				std::make_format_args());
		}
	};
}

namespace ArRobot {
	struct CheckDirData
	{
		std::int32_t x; 
		std::int32_t y;
		BlockType block;
	};

	struct MemCopyData
	{
		std::size_t destAddr;
		std::size_t srcAddr;
	};

	struct MemAddrAndValue
	{
		std::size_t addr;
		std::int32_t value;
	};

	struct BinaryOpData
	{
		OpCode opCode;
		std::size_t lhs;
		std::size_t rhs;
	};

	template <CommandType CmdType>
	auto Command::As() const
	{
		// if constexpr for the win, baby!
		using enum CommandType;
		if constexpr (
			CmdType == DoNothing || 
			CmdType == Halt || 
			CmdType == MemPrintAll ||
			CmdType == PickUp || 
			CmdType == Drop)
		{
			return nullptr;
		}
		else if constexpr (CmdType == Move)
		{
			return Arge::Point{m_IntData[0], m_IntData[1]};
		}
		else if constexpr (CmdType == CheckDir)
		{
			return CheckDirData{m_IntData[0], m_IntData[1], static_cast<BlockType>(m_IntData[2])};
		}
		else if constexpr (
			CmdType == MarkLabel || 
			CmdType == Jump || 
			CmdType == JumpTrue || 
			CmdType == JumpFalse)
		{
			return m_StringData;
		}
		else if constexpr (CmdType == MemSet || CmdType == Increment)
		{
			return MemAddrAndValue{static_cast<std::size_t>(m_IntData[0]), m_IntData[1]};
		}
		else if constexpr (CmdType == MemCopy)
		{
			return MemCopyData{
				static_cast<std::size_t>(m_IntData[0]),
				static_cast<std::size_t>(m_IntData[1]),
			};
		}
		else if constexpr (CmdType == BinaryOp)
		{
			return BinaryOpData{
				static_cast<OpCode>(m_IntData[0]),
				static_cast<std::size_t>(m_IntData[1]),
				static_cast<std::size_t>(m_IntData[2]),
			};
		}
		else if constexpr (CmdType == MemPrint)
		{
			return static_cast<std::size_t>(m_IntData[0]);
		}
		else
		{
			throw GenericError{"Invalid or Unimplemented CommandType passed to As<>"};
		}
	}
}