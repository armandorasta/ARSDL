#include "pch.hpp"
#include "Tokenizer.hpp"

namespace ArRobot {
	enum class Tokenizer::St : std::int32_t
	{
	};

	void Tokenizer::DoIteration()
	{
		if (auto const c {m_Code.front()}; std::isspace(c))
		{
			m_Code.remove_prefix(1);
		}
		else if (std::isdigit(c) || c == '-')
		{
			ParseNumber();
		}
		else if (std::isalpha(c) || c == '_')
		{
			ParseName();
		}
		else
		{
			ParseOperator();
		}
	}

	void Tokenizer::ParseName()
	{
		auto it {m_Code.cbegin()};
		for (; it < m_Code.end() && IsCharValidForIdent(*it); ++it)
		{
			AppendChar(*it);
		}


		if (auto const strAcc {GetStringAcc()}; KeywordTypeEnum::IsKeyword(strAcc))
		{
			AddToken(Token::MakeKeyword(strAcc));
		}
		else
		{
			AddToken(Token::MakeName(strAcc));
		}
		m_Code.remove_prefix(static_cast<std::size_t>(std::distance(m_Code.begin(), it)));
		ResetStringAcc();
	}

	void Tokenizer::ParseOperator()
	{
		AddToken([c = m_Code.front()] 
		{
			using enum TokenType;
			switch (c)
			{
			case '(':  return LeftPeren;
			case ')':  return RightPeren;
			case '#':  return Hash;
			case '[':  return LeftSquareBracket;
			case ']':  return RightSquareBracket;
			case '.':  return Dot;
			case ',':  return Comma;
			case '!':  return ExclamationMark;
			case '$':  return DollarSign;
			case ':':  return Colon;
			case ';':  return SemiColon;
			case '\'': return SingleQuote;
			case '"':  return DoubleQuote;
			default:
				throw ParseError{"Invalid token: {}; remove this token", c};
			}
		}(/*)(*/));
		m_Code.remove_prefix(1);
	}

	void Tokenizer::ParseNumber()
	{
		auto const res {m_NumberParser.Parse(m_Code)};
		AddToken(Token::MakeNum(res.num));
		m_Code.remove_prefix(res.offset);
	}

	bool Tokenizer::IsCharValidForIdent(char c) const
	{
		return std::isalnum(c) || c == '_';
	}

	std::string Tokenizer::FileToString(std::ifstream& file) const
	{
		file.seekg(0, std::ios::end);
		auto const fileSize {file.tellg()};
		file.seekg(0);
		std::string str(fileSize, '\0');
		file.read(str.data(), str.size());
		return str;
	}

	std::vector<Token> Tokenizer::Tokenize(std::ifstream& file)
	{
		return Tokenize(FileToString(file));
	}
	
	std::vector<Token> Tokenizer::Tokenize(std::string_view code)
	{
		m_Tokens.clear(); // Make sure the vector is in a valid state.
		m_Code = code;
		while (!m_Code.empty()) // Looks very silly, because it probably is.
		{
			DoIteration();
		}
		return std::move(m_Tokens); // So it resets automatically.
	}
}
