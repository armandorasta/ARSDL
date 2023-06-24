#include "pch.hpp"
#include "Token.hpp"
#include "ArRobotException.hpp"

namespace ArRobot {
	std::string Token::ToString() const
	{
		// I could implement a compile-time version of this, but I would have to 
		// used ten billion (if constexpr)s and I am not willing to do that for
		// a close to nothing performance gain :).
		using enum TokenType;
		switch (m_Type)
		{
		case Number:
			return std::format("Number({})", As<Number>().num);
		case Name:
			return std::format("Name({})", As<Name>().glyph);
		case Keyword:
			return std::format("Keyword({})", As<Keyword>().type);
		case None:               return "Token(None)";
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
		case Star:               return "*";
		default:
			throw GenericError{"(Invalid or unimplemented Token)"};
		}
	}

	std::ostream& operator<<(std::ostream& lhs, Token const& rhs)
	{
		return (lhs << rhs.ToString());
	}
}
