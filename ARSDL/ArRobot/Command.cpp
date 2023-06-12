#include "pch.h"
#include "Command.hpp"
#include "ArRobotException.hpp"

namespace ArRobot {
	std::string Command::ToString() const
	{
		using enum CommandType;
		switch (type)
		{
		case Move:      return std::format("[Move](x={}, y={})\n", data[0], data[1]);
		case PickUp:    return "[PickUp]()\n";
		case Drop:      return "[Drop]()\n";
		case Jump:      return std::format("[Jump](addr={})\n", data[0]);
		case JumpTrue:  return std::format("[JumpTrue](addr={})\n", data[0]);
		case JumpFalse: return std::format("[JumpFalse](addr={})\n", data[0]);
		case MemSet:    return std::format("[MemSet](addr={}, val={})\n", data[0], data[1]);
		case MemCopy:   return std::format("[MemCopy](to={}, from={})\n", data[0], data[1]);
		case Increment: return std::format("[Increment](addr={}, del={})\n", data[0], data[1]);
		case BinaryOp:  
		{
			auto const opCode{static_cast<OpCode>(data[0])};
			return std::format("[{}](lhs={}, rhs={})\n", opCode, data[1], data[2]);
		}
		case MemPrint:
		case MemPrintAll:
			// Their default behaviour is printing, no more info is needed.
			break;
		}
		
		// The default label will trigger the 'not all path return a value' warning,
		// for whatever reason.
		return "Invalid";
	}

	std::ostream& operator<<(std::ostream& lhs, Command const& rhs)
	{
		return (lhs << rhs.ToString());
	}
}
