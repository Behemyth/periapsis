#include <gtest/gtest.h>

import std;
import inplace_vector;

namespace
{
	TEST(InplaceVector, PushBackWithinCapacity)
	{
		std::inplace_vector<int, 4> vector;
		vector.push_back(1);
		vector.push_back(2);

		EXPECT_EQ(vector.size(), 2);
		EXPECT_EQ(vector.capacity(), 4);
		EXPECT_EQ(vector[0], 1);
		EXPECT_EQ(vector[1], 2);
	}

	TEST(InplaceVector, TryPushBackFailsWhenFull)
	{
		std::inplace_vector<int, 1> vector;
		EXPECT_NE(vector.try_push_back(1), nullptr);
		EXPECT_EQ(vector.try_push_back(2), nullptr);
		EXPECT_EQ(vector.size(), 1);
	}

	TEST(InplaceVector, ConstinitStorage)
	{
		constinit static std::inplace_vector<int*, 2> registry;
		static int value = 0;
		registry.push_back(&value);
		EXPECT_EQ(registry.size(), 1);
		registry.clear();
	}
}
