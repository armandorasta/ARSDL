#pragma once
#include "ArRobotCore.hpp"
#include "KeywordType.hpp"

namespace ArRobot {
	enum class TokenType : std::int32_t
	{
		None = 0,
		Number,
		Name,
		Keyword,

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
		DoubleQuote,
		SingleQuote,
		Star,
	};

	// Impl block for the TokenType enum
	namespace TokenTypeEnum {
		using enum TokenType;

		template <TokenType = static_cast<TokenType>(0)>
		struct Data {};

		template <> struct Data<Number>  { std::int32_t num;  };
		template <> struct Data<Name>    { std::string glyph; };
		template <> struct Data<Keyword> { KeywordType type;  };

		// TODO: change this into a runtime version rather than a template, so it does not explode.
		template <TokenType LeftTkt, TokenType RightTkt>
		constexpr bool operator==(Data<LeftTkt> const& lhs, Data<RightTkt> const& rhs) 
		{
			if constexpr (LeftTkt != RightTkt) 
			{
				return false;
			}

			if constexpr (LeftTkt == Number) 
			{
				return lhs.num == rhs.num;
			}
			else if constexpr (LeftTkt == Name)
			{
				return lhs.glyph == rhs.glyph;
			}
			else if constexpr (LeftTkt == Keyword)
			{
				return lhs.type == rhs.type;
			}
			else 
			{
				return true; // Out of bounds are gonna get me fucked up, but whatever.
			}
		}

		using Variant = std::variant<Data<>, Data<Number>, Data<Name>, Data<Keyword>>;
	}

	class Token
	{
	public:
		constexpr Token(TokenType tkt, TokenTypeEnum::Variant const& data = {}) 
			: m_Type{tkt}, m_Data{data}
		{
		}

		// Make the code look extra beautiful.
		constexpr Token(std::int32_t num) : Token {MakeNum(num)} {}
		constexpr Token(std::string_view name) : Token {MakeName(name)} {}
		// The compiler needs help with this for some reason -_-.
		constexpr Token(char const* name) : Token {MakeName(name)} {}
		constexpr Token(KeywordType keyword)
			: Token {TokenType::Keyword, TokenTypeEnum::Data<TokenType::Keyword>{keyword}}
		{
		}

		constexpr static Token MakeNum(std::int32_t num)
		{
			using enum TokenType;
			return Token{Number, TokenTypeEnum::Data<Number>{num}};
		}

		constexpr static Token MakeName(std::string_view name)
		{
			AROBOT_DA(!KeywordTypeEnum::IsKeyword(name), "Tried to add a keyword using a string?");
			using enum TokenType;
			return Token {Name, TokenTypeEnum::Data<Name>{std::string{name}}};
		}

		constexpr static Token MakeKeyword(std::string_view op)
		{
			using enum TokenType;
			return Token {Keyword, TokenTypeEnum::Data<Keyword>{KeywordTypeEnum::FromString(op)}};
		}

		constexpr TokenType GetType() const { return m_Type; }

		constexpr bool IsNumber() const
		{
			return std::holds_alternative<TokenTypeEnum::Data<TokenType::Number>>(m_Data);
		}

		constexpr bool IsName() const
		{
			return std::holds_alternative<TokenTypeEnum::Data<TokenType::Name>>(m_Data);
		}

		constexpr bool IsKeyword() const
		{
			return std::holds_alternative<TokenTypeEnum::Data<TokenType::Keyword>>(m_Data);
		}

		constexpr bool IsOperator() const
		{
			// TODO: Nothing needs to be said; simply make an OperatorType enumeration.
			return !(IsNumber() || IsName() || IsKeyword());
		}

		template <TokenType Type>
		constexpr auto const& As() const
		{
			return std::get<TokenTypeEnum::Data<Type>>(m_Data);
		}

		constexpr bool operator==(Token const& rhs) const
		{
			// Runtime type is just extra garbage.
			return m_Data == rhs.m_Data;
		}

		friend std::ostream& operator<<(std::ostream& lhs, Token const& rhs);

		std::string ToString() const;

	private:
		// The runtime type allows for avoiding using templates, and preventing the code
		// from exploding; it does not need to exist at all otherwise.
		TokenType m_Type;
		TokenTypeEnum::Variant m_Data;
	};
}

namespace std {
	template <>
	struct formatter<ArRobot::Token> : formatter<std::string>
	{
		auto format(ArRobot::Token const& tk, format_context context) const
		{
			return formatter<std::string>{}.format(tk.ToString(), context);
		}
	};
}