#include "boost/ut.hpp"
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

namespace ut = boost::ut;

ut::suite function_ref = [] {
	using namespace ut;

	"Construction"_test = [] {
		synodic::function_ref<void()> function1 = [] {
		};
		synodic::function_ref<void()> function2			  = FreeFunction;
		synodic::function_ref<void(TestStruct)> function3 = &TestStruct::MemberFunctionOne;
	};

	"Call"_test = [] {
		{
			const synodic::function_ref<void()> function = FreeFunction;
			function();
			expect(freeTest);
		}

		{
			TestStruct testStruct;
			auto testFunction								  = &TestStruct::MemberFunctionOne;
			synodic::function_ref<void(TestStruct&)> function = testFunction;
			function(testStruct);
			expect(testStruct.oneTest);
			testFunction = &TestStruct::MemberFunctionTwo;
			function	 = testFunction;
			function(testStruct);
			expect(testStruct.twoTest);
		}

		{
			auto testFunction = [] {
				return 42;
			};
			const synodic::function_ref<int()> function = testFunction;
			expect(function() == 42);
		}

		{
			int i			  = 0;
			auto testFunction = [&i] {
				i = 42;
			};
			const synodic::function_ref<void()> function = testFunction;
			function();
			expect(i == 42);
		}
	};

	"Assignment"_test = [] {
		{
			synodic::function_ref<void(void)> function = FreeFunction;
			function								   = [] {
			  };
		}

		{
			synodic::function_ref<void(TestStruct)> function = &TestStruct::MemberFunctionOne;
			function										 = &TestStruct::MemberFunctionTwo;
		}
	};

	"Deduction"_test = [] {
		{
			int i					= 5;
			synodic::function_ref g = [&](double) {
				return i;
			};
		}
	};
};