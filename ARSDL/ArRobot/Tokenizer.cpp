#include "pch.h"
#include "Tokenizer.hpp"

namespace ArRobot {
	enum class Tokenizer::St : std::int32_t
	{
		WhiteSpace,
		ParsingName,
		ParsingOperator,

		ParsingNumber,
		NumberParsingBegin,
		LeadingZero,
	};

	void Tokenizer::DoIteration(char c)
	{
		switch (GetCurrState())
		{
		case St::WhiteSpace:        ParseWhiteSpace(c); break;
		case St::ParsingName:       ParseName(c); break;
		case St::ParsingOperator:   ParseOperator(c); break;
		case St::ParsingNumber:     ParseNumber(c); break;
		default:                    AROBOT_UNREACHABLE_CODE();
		}
	}

	void Tokenizer::ParseWhiteSpace(char c)
	{
		if (std::isspace(c))
		{
			return;
		}
		else if (std::isalpha(c) || c == '_')
		{
			SetState(St::ParsingName);
			ParseName(c);
		}
		else if (std::isdigit(c) || c == '-')
		{
			SetState(St::ParsingNumber);
			ParseNumber(c);
		}
		else
		{
			SetState(St::ParsingOperator);
			ParseOperator(c);
		}
	}

	void Tokenizer::ParseName(char c)
	{
		if (IsCharValidForIdent(c)) // Parsing...
		{ 
			AppendChar(c);
		}
		else
		{
			// Finished parsing, evaluating...
			AddToken(Token::MakeName(GetStringAcc()));
			ResetStringAcc();
		}
	}

	void Tokenizer::ParseOperator(char c)
	{
		AddToken([=] {
			using enum TokenType;
			switch (c)
			{
			case '(': return LeftPeren;
			case ')': return RightPeren;
			case '#': return Hash;
			case '[': return LeftSquareBracket;
			case ']': return RightSquareBracket;
			case '.': return Dot;
			case ',': return Comma;
			case '!': return ExclamationMark;
			case '$': return DollarSign;
			case ':': return Colon;
			case ';': return SemiColon;
			default:
				throw ParseError{"Invalid token: {}; remove this token", c};
			}
		}(/*)(*/));
	}

	void Tokenizer::ParseNumber(char c)
	{
		if (c == '-')
		{
			if (!GetStringAcc().empty())
			{
				throw ParseError{"Expected a digit here, initiating a negative number"};
			}
			
			AppendChar(c);
			return;
		}
		else if (std::isxdigit(c))
		{
			AppendChar(c);
			return;
		}

		auto strAcc{GetStringAcc()};
		auto const bMinus{strAcc.front() == '-'};
		if (bMinus)
		{
			if (strAcc.size() == 1)
			{
				throw ParseError{"Expected a digit here, initiating a negative number"};
			}

			strAcc = strAcc.substr(1);
		}

		auto constexpr chToDigit = [](char ch) constexpr 
		{
			return static_cast<std::int32_t>(ch) - '0';
		};

		auto base = std::int32_t{10};
		auto st{St::NumberParsingBegin};
		auto num = std::int64_t{};
		for (auto const currCh : strAcc)
		{
			switch (st)
			{
			case St::NumberParsingBegin:
			{
				if (currCh == '0')
				{
					st = St::LeadingZero;
				}
				else
				{
					num = num * base + chToDigit(currCh);
					st = St::ParsingNumber;
				}
				break;
			}
			case St::LeadingZero:
			{
				if (std::isdigit(currCh))
				{
					num = num * base + chToDigit(currCh);
				}
				else switch (currCh)
				{
				case 'x': case 'X':
					base = 16;
					break;
				case 'b': case 'B':
					base = 2;
					break;
				default:
					throw ParseError{"Found character {} while parsing decimal number", c};
				}
				
				st = St::ParsingNumber;
				break;
			}
			case St::ParsingNumber:
			{
				if (!std::isxdigit(currCh))
				{
					auto const baseName = [base] {
						switch (base) 
						{
						case 2:  return "binary";
						case 10: return "decimal";
						case 16: return "hexadecimal";
						default: AROBOT_UNREACHABLE_CODE();
						}
					}(/*)(*/);
					throw ParseError{"Found character {} while parsing {} number", c, baseName};
				}
				
				auto const digit{chToDigit(currCh)};
				if (digit >= base)
				{
					throw ParseError{"Found digit {} while parsing number of base {}", digit, base};
				}

				num = num * base + chToDigit(currCh);
				break;
			}
			default:
				AROBOT_UNREACHABLE_CODE();
			}
		}

		SetState(St::WhiteSpace);
		DoIteration(c);
	}

	bool Tokenizer::IsCharValidForIdent(char c) const
	{
		return std::isalnum(c) || c == '_';
	}

	std::vector<Token> Tokenizer::Tokenize(std::ifstream& file)
	{
		file.seekg(0, std::ios::end);
		auto const fileSize{file.tellg()};
		file.seekg(0);
		std::string code(fileSize, '\0');
		file.read(code.data(), code.size());
		return Tokenize(code);
	}
	
	std::vector<Token> Tokenizer::Tokenize(std::string_view code)
	{
		for (auto const ch : code)
		{
			DoIteration(ch);
		}

		return {};
	}
}
