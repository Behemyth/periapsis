#include <gtest/gtest.h>

#include "synodic/functional/function_ref.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Types
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool freeTest = false;

void FreeFunction()
{
	freeTest = true;
}

struct TestStruct
{
	void MemberFunctionOne()
	{
		oneTest = true;
	}

	void MemberFunctionTwo()
	{
		twoTest = true;
	}

	bool oneTest = false;
	bool twoTest = false;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Tests
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

TEST(function_ref, Construction)
{
	synodic::function_ref function1 = []
	{
	};
	synodic::function_ref function2					  = FreeFunction;
	synodic::function_ref<void(TestStruct)> function3 = &TestStruct::MemberFunctionOne;
}

TEST(function_ref, Call)
{
	{
		const synodic::function_ref function = FreeFunction;
		function();
		EXPECT_TRUE(freeTest);
	}

	{
		TestStruct testStruct;
		auto testFunction								  = &TestStruct::MemberFunctionOne;
		synodic::function_ref<void(TestStruct&)> function = testFunction;
		function(testStruct);
		EXPECT_TRUE(testStruct.oneTest);

		testFunction = &TestStruct::MemberFunctionTwo;
		function	 = testFunction;
		function(testStruct);
		EXPECT_TRUE(testStruct.twoTest);
	}

	{
		auto testFunction = []
		{
			return 42;
		};
		const synodic::function_ref function = testFunction;
		EXPECT_TRUE(function() == 42);
	}

	{
		int i			  = 0;
		auto testFunction = [&i]
		{
			i = 42;
		};
		const synodic::function_ref function = testFunction;
		function();
		EXPECT_TRUE(i == 42);
	}
}

TEST(function_ref, Assignment)
{
	{
		synodic::function_ref function = FreeFunction;
		function					   = []
		{
		};
	}

	{
		synodic::function_ref<void(TestStruct)> function = &TestStruct::MemberFunctionOne;
		function										 = &TestStruct::MemberFunctionTwo;
	}
}

TEST(function_ref, Deduction)
{
	{
		const int i				= 5;
		synodic::function_ref g = [&](double)
		{
			return i;
		};
	}
}