#include "pch.h"
#include <Tokenizer.hpp>

#define TOKENIZER_TEST(_testName) TEST_F(TokenizerTests, _testName)
#define TOKEN_TEST(_testName)     TEST_F(TokenTests, _testName)

using namespace ArRobot;

class TokenTests : public ::testing::Test
{
};

class TokenizerTests : public ::testing::Test
{
public:
	static Tokenizer GenerateTestingInstance()
	{
		return Tokenizer {};
	}
};

TOKENIZER_TEST(Number_tokens)
{
	auto tokenizer {GenerateTestingInstance()};
	ASSERT_EQ(std::vector {Token::MakeNum(123)}, tokenizer.Tokenize("123"));
	ASSERT_EQ(std::vector {Token::MakeNum(0x12F)}, tokenizer.Tokenize("0x12F"));
	ASSERT_EQ(std::vector {Token::MakeNum(0b1101)}, tokenizer.Tokenize("0b1101"));
}

TOKENIZER_TEST(Name_tokens)
{
	auto tokenizer {GenerateTestingInstance()};
	auto const res = std::vector<Token> {"Hello", "this", "_a", "long123", "__Str_ing__"};
	ASSERT_EQ(res, tokenizer.Tokenize("Hello this _a long123 __Str_ing__"));
	ASSERT_ANY_THROW(tokenizer.Tokenize("5StartsWithANumber"));
}

TOKENIZER_TEST(Single_char_operator_tokens)
{
	using enum TokenType;
	auto tokenizer {GenerateTestingInstance()};
	auto const res = std::vector<Token> {
		LeftPeren, RightPeren, Hash, LeftSquareBracket, RightSquareBracket,
	};
	ASSERT_EQ(res, tokenizer.Tokenize("()#[]"));
}

TOKENIZER_TEST(Keywords)
{
	using enum KeywordType;
	auto tokenizer {GenerateTestingInstance()};
	auto const res = std::vector<Token> {Proc, Sticker, Enum};
	ASSERT_EQ(res, tokenizer.Tokenize("proc sticker enum"));
}

TOKENIZER_TEST(Random_mesh_of_tokens)
{
	using enum TokenType;
	auto tkz {GenerateTestingInstance()};
	auto const res = std::vector<Token> {
		LeftPeren, "Hello", RightPeren, Hash, LeftSquareBracket, 1234, RightSquareBracket,
		"Hello", "again", KeywordType::Sticker,
	};
	ASSERT_EQ(res, tkz.Tokenize("(Hello)#[1234] Hello again sticker"));
}

