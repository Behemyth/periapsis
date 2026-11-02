#include <gtest/gtest.h>

import std;
import function_ref;

namespace
{
	int Twice(int value)
	{
		return value * 2;
	}

	TEST(FunctionRef, InvokesLambda)
	{
		int calls = 0;
		auto lambda = [&](int value) {
			++calls;
			return value + 1;
		};

		std::function_ref<int(int)> ref(lambda);
		EXPECT_EQ(ref(1), 2);
		EXPECT_EQ(calls, 1);
	}

	TEST(FunctionRef, InvokesFreeFunction)
	{
		std::function_ref<int(int)> ref(Twice);
		EXPECT_EQ(ref(4), 8);
	}

	TEST(FunctionRef, InvokesNontypeBoundMember)
	{
		struct Counter
		{
			int value = 0;

			void Add(int amount)
			{
				value += amount;
			}
		};

		Counter counter;
		std::function_ref<void(int)> ref(std::nontype<&Counter::Add>, counter);
		ref(3);
		EXPECT_EQ(counter.value, 3);
	}
}
