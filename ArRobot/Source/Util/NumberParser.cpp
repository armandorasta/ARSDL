#include "pch.hpp"
#include "ArRobotCore.hpp"
#include "NumberParser.hpp"
#include "ArRobotException.hpp"

namespace ArRobot {
	void NumberParser::AppendChar(char c)
	{
		m_StringAcc.push_back(c);
	}

	std::string_view NumberParser::GetStringAcc() const
	{
		return m_StringAcc;
	}

	void NumberParser::ResetStringAcc()
	{
		m_StringAcc.clear();
	}

	std::int64_t NumberParser::ValidateDigit(char c, std::int64_t base)
	{
		switch (base)
		{
		case 2:
			if (!(c == '1' || c == '0'))
			{
				throw ParseError {"Found {} {} while parsing a binary number",
					std::isdigit(c) ? "digit" : "character"
				};
			}
			else
			{
				return c == '0' ? 0I64 : 1I64;
			}
		case 10:
			if (!std::isdigit(c))
			{
				throw ParseError {"Found character {} while parsing a decimal number", c};
			}
			else
			{
				return static_cast<std::int64_t>(c - '0');
			}
		case 16:
			if (!std::isxdigit(c))
			{
				throw ParseError {"Found character {} while parsing a hexadecimal number", c};
			}

			if (std::isdigit(c))
			{
				return static_cast<std::int64_t>(c - '0');
			}
			else if (std::tolower(c) == c)
			{
				return (c - 'a') + 10I64;
			}
			else
			{
				return (c - 'A') + 10I64;
			}
			break;
		default:
			AROBOT_UNREACHABLE_CODE();
		}
	}

	NumberParserResult NumberParser::Parse(std::string_view numStr)
	{
		// numStr will start with a number, but it may contain extra stuff at the end.
		AROBOT_DA(!numStr.empty());

		auto const bMinus {numStr.front() == '-'};
		if (bMinus)
		{
			numStr.remove_prefix(1);
			if (numStr.empty())
			{
				throw ParseError {"Expected a digit initiating a negative number"};
			}
		}

		auto resNum = std::int64_t {};
		auto base = decltype(resNum) {10};
		auto st {St::ParsingBegin};
		auto offset = std::size_t {};
		for (; offset < numStr.size(); ++offset)
		{
			auto const c {numStr[offset]};
			if (std::isspace(c) || !std::isalnum(c))
			{
				// It should stop without throwing when it finds a symbol like: + * \n.
				break;
			}

			switch (st)
			{
			case St::ParsingBegin:
				if (!std::isdigit(c))
				{
					throw ParseError {"Expected a digit initiating a number"};
				}

				if (c == '0')
				{
					st = St::LeadingZero;
				}
				else
				{
					resNum = static_cast<std::int64_t>(c - '0');
					st = St::Default;
				}
				break;
			case St::LeadingZero:
				if (std::isdigit(c))
				{
					resNum += resNum * 10 + static_cast<std::int64_t>(c - '0');
				}
				else switch (c)
				{
 				case 'b': case 'B': base = 2;  break;
				case 'x': case 'X': base = 16; break;
				default:
					throw ParseError {"Found character {} while parsing number"};
				}

				st = St::Default;
				break;
			case St::Default:
				resNum = resNum * base + ValidateDigit(c, base);
				break;
			default:
				AROBOT_UNREACHABLE_CODE();
			}
		}

		resNum *= bMinus ? -1 : 1;
		if (resNum < std::numeric_limits<std::int32_t>::min() ||
			resNum > std::numeric_limits<std::int32_t>::max())
		{
			throw ParseError {"Number {} cannot fit in 32 bits"};
		}

		return {
			.offset = offset,
			.num    = static_cast<std::int32_t>(resNum),
		};
	}

}