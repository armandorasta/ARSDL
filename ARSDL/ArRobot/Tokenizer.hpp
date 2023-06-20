#pragma once
#include "Arge.hpp"
#include "Command.hpp"
#include "Token.hpp"
#include "Util/NumberParser.hpp"

namespace ArRobot {
	class Tokenizer
	{
		enum class St : std::int32_t;
	public:
		Tokenizer() = default;

	private:
		void DoIteration(char c);
		void ParseWhiteSpace(char c);
		void ParseNumber(char c);
		void ParseName(char c);
		void ParseOperator(char c);

		constexpr void SetState(St newState)
		{
			m_CurrState = newState;
		}

		constexpr St GetCurrState() const
		{
			return m_CurrState;
		}

		constexpr void AppendChar(char ch)
		{
			m_CurrStringAcc.push_back(ch);
		}

		constexpr std::string_view GetStringAcc() const
		{
			return m_CurrStringAcc;
		}

		constexpr void ResetStringAcc()
		{
			m_CurrStringAcc.clear();
		}

		constexpr void AddToken(Token const& tk)
		{
			m_Tokens.emplace_back(tk);
		}

		bool IsCharValidForIdent(char c) const;


	public:
		std::vector<Token> Tokenize(std::ifstream& file);
		std::vector<Token> Tokenize(std::string_view code);



	private:
		std::string_view m_Code{};
		St m_CurrState{};
		std::string m_CurrStringAcc{};
		std::vector<Token> m_Tokens{};
	};
}