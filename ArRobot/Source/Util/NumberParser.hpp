#pragma once
#include "pch.hpp"
#include "ArRobotCore.hpp"

namespace ArRobot {
	struct NumberParserResult
	{
		std::size_t offset;
		std::int32_t num;
	};

	class NumberParser
	{
	private:
		enum class St : std::int32_t
		{
			Default,
			ParsingBegin,
			LeadingZero,
		};

		void AppendChar(char c);
		std::string_view GetStringAcc() const;
		void ResetStringAcc();
		std::int64_t ValidateDigit(char c, std::int64_t base);

	public:
		constexpr NumberParser() = default;

		NumberParserResult Parse(std::string_view numStr);
		constexpr auto const& GetState() const
		{
			return m_State;
		}

	private:
		std::optional<std::int32_t> m_State {};
		std::string m_StringAcc {};
	};
}