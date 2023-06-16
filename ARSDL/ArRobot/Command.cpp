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
		case Move:      return std::format("[Move](x={}, y={})\n", m_IntData[0], m_IntData[1]);
		case PickUp:    return "[PickUp]()\n";
		case Drop:      return "[Drop]()\n";
		case MarkLabel: return std::format("[MarkLabel](name={})\n", m_StringData);
		case Jump:      return std::format("[Jump](addr={})\n", m_IntData[0]);
		case JumpTrue:  return std::format("[JumpTrue](addr={})\n", m_IntData[0]);
		case JumpFalse: return std::format("[JumpFalse](addr={})\n", m_IntData[0]);
		case Halt:      return "Halt\n";
		case MemSet:    return std::format("[MemSet](addr={}, val={})\n", m_IntData[0], m_IntData[1]);
		case MemCopy:   return std::format("[MemCopy](to={}, from={})\n", m_IntData[0], m_IntData[1]);
		case Increment: return std::format("[Increment](addr={}, del={})\n", m_IntData[0], m_IntData[1]);
		case BinaryOp:  
		{
			auto const opCode{static_cast<OpCode>(m_IntData[0])};
			return std::format("[{}](lhs={}, rhs={})\n", opCode, m_IntData[1], m_IntData[2]);
		}
		case CheckDir:  return std::format(
			"[CheckDir](x={}, y={}, block={})\n", 
			m_IntData[0], m_IntData[1], static_cast<BlockType>(m_IntData[2]));
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
