#pragma once
#include "../Arge/ArgeCore.hpp"

namespace ArRobot {
	class NumberParser
	{
	private:
		enum class St : std::int32_t
		{
			Default,
			LeadingZero,
		};

		void AppendChar(char c);
		std::string_view GetStringAcc() const;
		void ResetStringAcc();
		std::int64_t ValidateDigit(char c, std::int64_t base);

	public:
		constexpr NumberParser() = default;

		std::optional<std::int32_t> Parse(char c);

	private:
		std::string m_StringAcc {};
	};
} // namespace ArRobot