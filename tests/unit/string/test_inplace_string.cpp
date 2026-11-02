/**
 * @brief Unit tests for BasicInplaceString
 */

#include <gtest/gtest.h>

import std;
import synodic.periapsis;

namespace
{
	using namespace peri;

	TEST(InplaceString, FromLiteral)
	{
		constexpr InplaceString<16> string = "periapsis";
		EXPECT_EQ(string.view(), "periapsis");
	}

	TEST(InplaceString, FromStringView)
	{
		const std::string_view source = "orbit";
		InplaceString<8> string(source);

		EXPECT_EQ(string.size(), source.size());
		EXPECT_EQ(string.view(), source);
		EXPECT_EQ(string.c_str()[source.size()], '\0');
	}

	TEST(InplaceString, FromStringViewAtCapacity)
	{
		const std::string_view source = "12345678";
		InplaceString<8> string(source);

		EXPECT_EQ(string.size(), 8);
		EXPECT_EQ(string.view(), source);
	}

	TEST(InplaceString, Empty)
	{
		InplaceString<4> string;

		EXPECT_EQ(string.size(), 0);
		EXPECT_TRUE(string.view().empty());
	}
}
