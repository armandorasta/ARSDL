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

	namespace Cmd {
		struct DoNothing {};
		struct Move { std::int32_t x; std::int32_t y; };
		struct PickUp {};
		struct Drop {};
		struct CheckDir { std::int32_t x; std::int32_t y; BlockType block; };
		struct MarkLabel { std::string label; };
		struct Jump { std::string label; };
		struct JumpTrue { std::string label; };
		struct JumpFalse { std::string label; };
		struct Halt {};
		struct MemSet { std::size_t addr; std::int32_t value; };
		struct MemCopy { std::size_t toAddr; std::size_t fromAddr; };
		struct Increment { std::size_t addr; std::int32_t value; };
		struct BinaryOp { OpCode opCode; std::size_t lhsAddr; std::size_t rhsAddr; };
		struct MemPrint { std::size_t addr; };
		struct MemPrintAll {};

		template <CommandType CmdType>
		struct Data;

		template <> struct Data<CommandType::DoNothing> { using type = DoNothing; };
		
		template <> struct Data<CommandType::Move> { using type = Move; };
		template <> struct Data<CommandType::PickUp> { using type = PickUp; };
		template <> struct Data<CommandType::Drop> { using type = Drop; };

		template <> struct Data<CommandType::CheckDir> { using type = CheckDir; };
		template <> struct Data<CommandType::MarkLabel> { using type = MarkLabel; };
		template <> struct Data<CommandType::Jump> { using type = Jump; };
		template <> struct Data<CommandType::JumpTrue> { using type = JumpTrue; };
		template <> struct Data<CommandType::JumpFalse> { using type = JumpFalse; };
		template <> struct Data<CommandType::Halt> { using type = Halt; };

		template <> struct Data<CommandType::MemSet> { using type = MemSet; };
		template <> struct Data<CommandType::MemCopy> { using type = MemCopy; };
		template <> struct Data<CommandType::Increment> { using type = Increment; };
		template <> struct Data<CommandType::BinaryOp> { using type = BinaryOp; };

		template <> struct Data<CommandType::MemPrint> { using type = MemPrint; };
		template <> struct Data<CommandType::MemPrintAll> { using type = MemPrintAll; };

		template <CommandType CmdType>
		using Data_t = typename Data<CmdType>::type;

		using Variant = std::variant<
			DoNothing, 
			Move, PickUp, Drop, 
			CheckDir, MarkLabel, Jump, JumpTrue, JumpFalse, Halt,
			MemSet, MemCopy, Increment, BinaryOp,
			MemPrint, MemPrintAll
		>;
	}

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

		constexpr Command(CommandType type, Cmd::Variant var, std::size_t ticks = VeryShortPeriod)
			: m_Type{type}, m_Data{var}, m_Ticks{ticks}
		{
		}

		static constexpr Command MakeDoNothing()
		{
			return {CommandType::DoNothing, Cmd::DoNothing{}};
		}

		static constexpr Command MakeMove(std::int32_t delX, std::int32_t delY)
		{
			return {CommandType::Move, Cmd::Move{delX, delY}, LongPeriod};
		}

		static constexpr Command MakePickUp()
		{
			return {CommandType::PickUp, Cmd::PickUp{}, MediumPeriod};
		}

		static constexpr Command MakeDrop()
		{
			return {CommandType::Drop, Cmd::Drop{}, MediumPeriod};
		}

		static constexpr Command MakeCheckDir(std::int32_t x, std::int32_t y, BlockType whatToCheckFor)
		{
			return {
				CommandType::CheckDir, 
				Cmd::CheckDir{x, y, whatToCheckFor},
				ShortPeriod,
			};
		}

		static constexpr Command MakeMarkLabel(std::string_view name)
		{
			return {CommandType::MarkLabel, Cmd::MarkLabel{std::string{name}}, InstantPeriod};
		}

		static constexpr Command MakeJump(std::string_view label)
		{
			return {CommandType::Jump, Cmd::Jump{std::string{label}}, VeryShortPeriod};
		}

		static constexpr Command MakeJumpTrue(std::string_view label)
		{
			return {CommandType::JumpTrue, Cmd::JumpTrue{std::string{label}}, VeryShortPeriod};
		}

		static constexpr Command MakeJumpFalse(std::string_view label)
		{
			return {CommandType::JumpFalse, Cmd::JumpFalse{std::string{label}}, VeryShortPeriod};
		}

		static constexpr Command MakeHalt()
		{
			return {CommandType::Halt, Cmd::Halt{}};
		}

		static constexpr Command MakeMemSet(std::size_t address, std::int32_t value)
		{
			return {CommandType::MemSet, Cmd::MemSet{address, value}, ShortPeriod};
		}

		static constexpr Command MakeMemCopy(std::size_t to, std::size_t from)
		{
			return {CommandType::MemCopy, Cmd::MemCopy{to, from}, ShortPeriod};
		}

		static constexpr Command MakeIncrement(std::size_t address, std::int32_t incValue)
		{
			return {CommandType::Increment, Cmd::Increment{address, incValue}, ShortPeriod};
		}

		// Stores in lhs.
		static constexpr Command MakeBinaryOp(OpCode opCode, std::size_t lhsAddress, 
			std::size_t rhsAddress)
		{
			return {
				CommandType::BinaryOp,
				Cmd::BinaryOp{
					opCode,
					lhsAddress,
					rhsAddress,
				},
				ShortPeriod,
			};
		}

		static constexpr Command MakeMemPrint(std::size_t address)
		{
			return {CommandType::MemPrint, Cmd::MemPrint{address}, VeryShortPeriod};
		}

		static constexpr Command MakeMemPrintAll()
		{
			return {CommandType::MemPrintAll, Cmd::MemPrintAll{}, VeryShortPeriod};
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
			return std::get<Cmd::Data_t<CmdType>>(m_Data);
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

/*
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
*/