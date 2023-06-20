#include "pch.h"
#include <Util/NumberParser.hpp>

#define NUM_PARSER_TEST(_testName) TEST_F(NumberParserTests, _testName)

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
};


NUM_PARSER_TEST(TestName)
{
	
}