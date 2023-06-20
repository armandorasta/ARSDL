#include "pch.h"
#include "Token.hpp"
#include "ArRobotException.hpp"

namespace ArRobot {
	std::string Token::ToString() const
	{
		using enum TokenType;
		switch (m_Type)
		{
		case Number: return std::format("Number({})", As<Number>().num);
		case Name:   return std::format("Name({})", As<Name>().glyph);

		case LeftPeren:          return "(";
		case RightPeren:         return ")";
		case Hash:               return "#";
		case LeftSquareBracket:  return "[";
		case RightSquareBracket: return "]";
		case Dot:                return ".";
		case Comma:              return ",";
		case ExclamationMark:    return "!";
		case DollarSign:         return "$";
		case Colon:              return ":";
		case SemiColon:          return ";";
		default:           
			throw GenericError{"Token::ToString on Invalid Token"};
		}
	}
}
