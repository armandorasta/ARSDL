#pragma once
#include "ArRobotCore.hpp"

namespace ArRobot {
	enum class TokenType : std::int32_t
	{
		None = 0,
		Number,
		Name,

		LeftPeren,
		RightPeren,
		Hash,
		LeftSquareBracket,
		RightSquareBracket,
		Dot,
		Comma,
		ExclamationMark,
		DollarSign,
		Colon,
		SemiColon,
	};

	// Impl block for the TokenType enum
	namespace TokenTypeEnum {
		using enum TokenType;

		template <TokenType = static_cast<TokenType>(0)>
		struct Data {};

		template <> struct Data<Number> { std::int32_t num; };
		template <> struct Data<Name>   { std::string glyph; };

		using Variant = std::variant<Data<>, Data<Number>, Data<Name>>;
	}

	class Token
	{
	public:
		Token(TokenType tkt, TokenTypeEnum::Variant const& data = {}) 
			: m_Type{tkt}, m_Data{data}
		{
		}

		static Token MakeNum(std::int32_t num)
		{
			return Token{TokenType::Number, TokenTypeEnum::Data<TokenType::Number>{num}};
		}

		static Token MakeName(std::string_view name)
		{
			return Token{TokenType::Name, TokenTypeEnum::Data<TokenType::Name>{std::string{name}}};
		}

		constexpr TokenType GetType() const
		{
			return m_Type;
		}

		template <TokenType Type>
		constexpr auto const& As() const
		{
			return std::get<TokenTypeEnum::Data<Type>>(m_Data);
		}

		std::string ToString() const;

	private:
		TokenType m_Type;
		TokenTypeEnum::Variant m_Data;
	};
}

namespace std {
	template <>
	struct formatter<ArRobot::Token> : formatter<std::string>
	{
		auto format(ArRobot::Token tk, format_context context) const
		{
			return formatter<std::string>{}.format(tk.ToString(), context);
		}
	};
}