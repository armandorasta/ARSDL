#include "pch.h"
#include <Util/NumberParser.hpp>

#define NUM_PARSER_TEST(_testName) TEST_F(NumberParserTests, _testName)

using namespace ArRobot;

class NumberParserTests : public ::testing::Test
{
public:
	void SetUp() override
	{
		(void) 0;
	}

	void TearDown() override
	{
		(void) 0;
	}

	static NumberParser GenerateTestingInstance()
	{
		return NumberParser {};
	}
};


NUM_PARSER_TEST(Decimal_numbers_average_case)
{
	auto numPar {GenerateTestingInstance()};
	ASSERT_EQ(1234, numPar.Parse("1234").num); // Average case.
	ASSERT_EQ(-1234, numPar.Parse("-1234").num); // Negative number.
	ASSERT_ANY_THROW(numPar.Parse("12F34").num); // Hex digit in decimal number.
	ASSERT_ANY_THROW(numPar.Parse("12K34").num); // Invalid character.
}

NUM_PARSER_TEST(Decimal_numbers_leading_zero)
{
	auto numPar {GenerateTestingInstance()};
	ASSERT_EQ(1234, numPar.Parse("01234").num);
	ASSERT_EQ(1234, numPar.Parse("0001234").num);
	ASSERT_EQ(-1234, numPar.Parse("-0001234").num);
	ASSERT_ANY_THROW(numPar.Parse("012F34").num); // Hex digit in decimal number.
	ASSERT_ANY_THROW(numPar.Parse("00012K34").num); // Invalid character.
}

NUM_PARSER_TEST(Binary_numbers)
{
	auto numPar {GenerateTestingInstance()};
	ASSERT_EQ(0b101, numPar.Parse("0b101").num); // Average case 
	ASSERT_EQ(-0b101, numPar.Parse("-0b101").num); // Average case 
	ASSERT_ANY_THROW(numPar.Parse("0b123"));  // Invalid binary digit
	ASSERT_ANY_THROW(numPar.Parse("0b12F3")); //
	ASSERT_ANY_THROW(numPar.Parse("0b12K3")); //

	// Hex
	ASSERT_EQ(0x15F0, numPar.Parse("0x15F0").num); // Average case 
	ASSERT_EQ(-0x15F0, numPar.Parse("-0x15F0").num); // Average case 
	ASSERT_ANY_THROW(numPar.Parse("0x12K3")); // Invalid hex digit
}

NUM_PARSER_TEST(Hexadecimal_numbers)
{
	auto numPar {GenerateTestingInstance()};
	ASSERT_EQ(0x15F0, numPar.Parse("0x15F0").num); // Average case 
	ASSERT_EQ(-0x15F0, numPar.Parse("-0x15F0").num); // Average case 
	ASSERT_ANY_THROW(numPar.Parse("0x12K3")); // Invalid hex digit
}

NUM_PARSER_TEST(Overflow)
{
	auto numPar {GenerateTestingInstance()};
	ASSERT_ANY_THROW(numPar.Parse("5000000000")); // Decimal, 5 billion.
	ASSERT_ANY_THROW(numPar.Parse("0b100000000000000000000000000000000")); // Binary, 2^32
	ASSERT_ANY_THROW(numPar.Parse("0xFFFFFFFFF")); // Hex, 4.5 bytes.
}

NUM_PARSER_TEST(Offset_returned)
{
	auto numPar {GenerateTestingInstance()};
	ASSERT_EQ(4, numPar.Parse("1234       1234").offset);
	ASSERT_EQ(4, numPar.Parse("1234*      1234").offset);
	ASSERT_EQ(4, numPar.Parse("1234\n     1234").offset);
	ASSERT_EQ(4, numPar.Parse("0xFF\n     1234").offset);
}