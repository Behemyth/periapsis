#include <vector>

#include "boost/ut.hpp"
#include "synodic/type_traits.h"


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Types
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct TestData
{
};

class TestClass
{
public:
	TestClass(std::vector<int> parameter1, TestData parameter2, int parameter3) :
		data1_(std::move(parameter1)), data2_(parameter2), data3_(parameter3)
	{
	}

	[[nodiscard]] int GetData3() const
	{
		return data3_;
	}

private:
	std::vector<int> data1_;
	TestData data2_;
	int data3_;
};

class ConstTestClass
{
public:
	ConstTestClass(
		const std::vector<int> parameter1,
		const TestData parameter2,
		const int parameter3) :
		data1_(parameter1),
		data2_(parameter2), data3_(parameter3)
	{
	}

private:
	const std::vector<int> data1_;
	const TestData data2_;
	const int data3_;
};

class ReferenceTestClass
{
public:
	ReferenceTestClass(std::vector<int>& parameter1, TestData& parameter2, int& parameter3) :
		data1_(parameter1), data2_(parameter2), data3_(parameter3)
	{
	}

private:
	std::vector<int>& data1_;
	TestData& data2_;
	int& data3_;
};

class PointerTestClass
{
public:
	PointerTestClass(std::vector<int>* parameter1, TestData* parameter2, int* parameter3) :
		data1_(parameter1), data2_(parameter2), data3_(parameter3)
	{
	}

private:
	std::vector<int>* data1_;
	TestData* data2_;
	int* data3_;
};


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Tests
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

TEST_SUITE("Reflection")
{

	TEST_CASE("Value ConstructorParameters")
	{
		using ConstructorType = synodic::ConstructorParameters<TestClass>;
		static_assert(std::is_same_v<ConstructorType, std::tuple<std::vector<int>, TestData, int>>);
	}

	TEST_CASE("Const ConstructorParameters")
	{
		using ConstructorType = synodic::ConstructorParameters<ConstTestClass>;
		static_assert(std::is_same_v<ConstructorType, std::tuple<std::vector<int>, TestData, int>>);
	}

	// TEST_CASE("Reference ConstructorParameters")
	//{
	//	using ConstructorType = synodic::ConstructorParameters<ReferenceTestClass>;
	//	static_assert(std::is_same_v<ConstructorType, std::tuple<std::vector<int>, TestData, int>>);
	//}

	// TEST_CASE("Pointer ConstructorParameters")
	//{
	//	using ConstructorType = synodic::ConstructorParameters<PointerTestClass>;
	//	static_assert(std::is_same_v<ConstructorType, std::tuple<std::vector<int>, TestData, int>>);
	//}
}