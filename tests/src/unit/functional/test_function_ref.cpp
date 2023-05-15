#include <gtest/gtest.h>

import Function;

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
	synodic::FunctionRef function1 = []()
	{
	};
	synodic::FunctionRef function2					 = FreeFunction;
	synodic::FunctionRef<void(TestStruct)> function3 = &TestStruct::MemberFunctionOne;
}

TEST(function_ref, Call)
{
	{
		const synodic::FunctionRef function = FreeFunction;
		function();
		EXPECT_TRUE(freeTest);
	}

	{
		TestStruct testStruct;
		auto testFunction								 = &TestStruct::MemberFunctionOne;
		synodic::FunctionRef<void(TestStruct&)> function = testFunction;
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
		const synodic::FunctionRef function = testFunction;
		EXPECT_TRUE(function() == 42);
	}

	{
		int i			  = 0;
		auto testFunction = [&i]
		{
			i = 42;
		};
		const synodic::FunctionRef function = testFunction;
		function();
		EXPECT_TRUE(i == 42);
	}
}

TEST(function_ref, Assignment)
{
	{
		synodic::FunctionRef function = FreeFunction;
		function					  = []
		{
		};
	}

	{
		synodic::FunctionRef<void(TestStruct)> function = &TestStruct::MemberFunctionOne;
		function										= &TestStruct::MemberFunctionTwo;
	}
}

TEST(function_ref, Deduction)
{
	{
		const int i			   = 5;
		synodic::FunctionRef g = [&](double)
		{
			return i;
		};
	}
}