#include "pch.h"
#include "ArRobotException.hpp"

namespace ArRobot {
	ArRobotException::ArRobotException() : ArRobotException{"No message"}
	{
	}

	std::string const& ArRobotException::GetMessage() const noexcept
	{
		return m_Message;
	}

	std::string& ArRobotException::GetMessage() noexcept
	{
		return m_Message;
	}
}