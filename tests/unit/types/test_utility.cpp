/**
 * @brief Unit tests for utility functions
 */

#include <gtest/gtest.h>

import std;
import synodic.periapsis;

namespace
{
	TEST(Utility, ExportsZStringView)
	{
		constexpr std::zstring_view view("hello");
		static_assert(view.size() == 5);
	}

	TEST(Utility, NthElement)
	{
		static_assert(1 == peri::nth_element<0>(1));
		static_assert(1 == peri::nth_element<0>(1, 2));
		static_assert(2 == peri::nth_element<1>(1, 2));
		static_assert(1 == peri::nth_element<0>(1, 2, 3));
		static_assert(2 == peri::nth_element<1>(1, 2, 3));
	}

	TEST(Utility, BasicHashFromString)
	{
		constexpr peri::Hash hash("root");
		static_assert(static_cast<std::uint32_t>(hash) == peri::FNV1a<std::uint32_t>::Hash("root"));
		static_assert(peri::Hash("root") != peri::Hash("child"));

		constexpr peri::Hash64 hash64("root");
		static_assert(static_cast<std::uint64_t>(hash64) == peri::FNV1a<std::uint64_t>::Hash("root"));
	}

	TEST(Utility, ExportsPolyfills)
	{
		std::inplace_vector<int, 2> vector;
		vector.push_back(1);

		auto lambda = [] { return 7; };
		std::function_ref<int()> ref(lambda);

		EXPECT_EQ(vector.size(), 1);
		EXPECT_EQ(ref(), 7);
	}
}
