#include "catch2/catch.hpp"
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

TEST_CASE("Construction", "[function_ref]")
{
	synodic::function_ref function1 = []
	{
	};
	synodic::function_ref function2					  = FreeFunction;
	synodic::function_ref<void(TestStruct)> function3 = &TestStruct::MemberFunctionOne;
}

TEST_CASE("Call", "[function_ref]")
{
	{
		const synodic::function_ref function = FreeFunction;
		function();
		REQUIRE(freeTest);
	}

	{
		TestStruct testStruct;
		auto testFunction								  = &TestStruct::MemberFunctionOne;
		synodic::function_ref<void(TestStruct&)> function = testFunction;
		function(testStruct);
		REQUIRE(testStruct.oneTest);

		testFunction = &TestStruct::MemberFunctionTwo;
		function	 = testFunction;
		function(testStruct);
		REQUIRE(testStruct.twoTest);
	}

	{
		auto testFunction = []
		{
			return 42;
		};
		const synodic::function_ref function = testFunction;
		REQUIRE(function() == 42);
	}

	{
		int i			  = 0;
		auto testFunction = [&i]
		{
			i = 42;
		};
		const synodic::function_ref function = testFunction;
		function();
		REQUIRE(i == 42);
	}
}

TEST_CASE("Assignment", "[function_ref]")
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

TEST_CASE("Deduction", "[function_ref]")
{
	{
		const int i				= 5;
		synodic::function_ref g = [&](double)
		{
			return i;
		};
	}
}