#include "Arge.hpp"
#include "NumberParser.hpp"
#include "../ArRobotException.hpp"

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
			if (c != '1' && c != '2')
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
			else if (std::isdigit(c))
			{
				return static_cast<std::int64_t>(c - '0');
			}
			else if (std::tolower(c) == c)
			{
				return static_cast<std::int64_t>(c - 'a') + 10I64;
			}
			else
			{
				return static_cast<std::int64_t>(c - 'A') + 10I64;
			}
			break;
		default:
			AROBOT_UNREACHABLE_CODE();
		}
	}

	std::optional<std::int32_t> NumberParser::Parse(char c)
	{
		if (c == '-')
		{
			if (GetStringAcc().empty())
			{
				AppendChar(c);
				return {};
			}

			throw ParseError { "Expected a digit initiating a negative number" };
		}
		else if (std::isxdigit(c))
		{
			AppendChar(c);
			return {};
		}

		// Validation phase...
		auto strAcc {GetStringAcc()};
		auto const bMinus {strAcc.front() == '-'};
		if (bMinus)
		{
			strAcc.remove_prefix(1);
			if (strAcc.empty())
			{
				throw ParseError { "Expected a digit initiating a negative number" };
			}
		}

		auto st {St::Default};
		if (strAcc.front() == '0')
		{
			st = St::LeadingZero;
			strAcc.remove_prefix(1);
		}

		auto resNum = std::int64_t {};
		auto base = decltype(resNum) { 10 };
		for (auto const currCh : strAcc)
		{
			using enum St;
			switch (st)
			{
			case Default:
				resNum = resNum * base + ValidateDigit(currCh, base);
				break;
			case LeadingZero:
				if (std::isdigit(currCh))
				{
					resNum = resNum * 10 + static_cast<std::int64_t>(currCh - '0');
				}
				else if (currCh == 'b' || currCh == 'B') // A switch will trigger sonar.
				{
					base = 2;
				}
				else if (currCh == 'x' || currCh == 'X')
				{
					base = 16;
				}
				else
				{
					throw ParseError {"Found character {} while parsing number"};
				}

				st = Default;
				break;
			default:
				AROBOT_UNREACHABLE_CODE();
			}
		}
	}
}