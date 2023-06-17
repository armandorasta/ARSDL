#include "pch.h"
#include "Command.hpp"
#include "ArRobotException.hpp"

namespace ArRobot {
	std::string Command::ToString() const
	{
		using enum CommandType;
		switch (m_Type)
		{
		case DoNothing: return "DoNothing\n";
		case Move:      return std::format("[Move](x={}, y={})\n", As<Move>().x, As<Move>().y);
		case PickUp:    return "[PickUp]()\n";
		case Drop:      return "[Drop]()\n";
		case MarkLabel: return std::format("[MarkLabel](name={})\n", As<MarkLabel>().label);
		case Jump:      return std::format("[Jump](addr={})\n", As<Jump>().label);
		case JumpTrue:  return std::format("[JumpTrue](addr={})\n", As<JumpTrue>().label);
		case JumpFalse: return std::format("[JumpFalse](addr={})\n", As<JumpFalse>().label);
		case Halt:      return "Halt\n";
		case MemSet:    return std::format("[MemSet](addr={}, val={})\n", As<MemSet>().addr, As<MemSet>().value);
		case MemCopy:   return std::format("[MemCopy](to={}, from={})\n", As<MemCopy>().toAddr, As<MemCopy>().fromAddr);
		case Increment: return std::format("[Increment](addr={}, del={})\n", As<Increment>().addr, As<Increment>().value);
		case BinaryOp:  
		{
			auto const& [opCode, lhs, rhs] { As<BinaryOp>() };
			return std::format("[{}](lhs={}, rhs={})\n", opCode, lhs, rhs);
		}
		case CheckDir:  
		{
			auto const& [x, y, block] { As<CheckDir>() };
			return std::format("[CheckDir](x={}, y={}, block={})\n", x, y, block);
		}
		case MemPrint:
		case MemPrintAll:
			// Their default behaviour is printing, no more info is needed.
			return "";
		}
		
		// The default label will trigger the "not all path return a value" warning,
		// for whatever reason.
		return "Invalid";
	}

	std::ostream& operator<<(std::ostream& lhs, Command const& rhs)
	{
		return (lhs << rhs.ToString());
	}
}
