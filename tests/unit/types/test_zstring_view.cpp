#include <gtest/gtest.h>

import std;
import zstring_view;

namespace
{
	TEST(ZStringView, PreservesNullTermination)
	{
		constexpr std::zstring_view view("hello");
		static_assert(view.size() == 5);
		static_assert(view.c_str()[view.size()] == '\0');
		EXPECT_EQ(std::string_view(view), "hello");
	}

	TEST(ZStringView, RemovePrefixPreservesTerminator)
	{
		std::zstring_view view("hello");
		view.remove_prefix(2);
		EXPECT_EQ(std::string_view(view), "llo");
		EXPECT_EQ(view.c_str()[view.size()], '\0');
	}
}