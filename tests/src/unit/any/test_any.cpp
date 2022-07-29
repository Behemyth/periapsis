#include <any>
#include <catch2/benchmark/catch_benchmark.hpp>
#include <catch2/catch_template_test_macros.hpp>

#include "Types_Any.h"

//====================================================================================================
// Helper Code
//====================================================================================================

/**
 * \brief Empty object that counts static lifetime of its instances
 */
class TrivialClass
{
public:
	TrivialClass() noexcept
	{
		referenceCount_++;
	}
	TrivialClass(const TrivialClass&) noexcept
	{
		referenceCount_++;
	}
	TrivialClass(TrivialClass&&) noexcept
	{
		referenceCount_++;
	}
	TrivialClass& operator=(const TrivialClass&) noexcept
	{
		referenceCount_++;
		return *this;
	}
	~TrivialClass() noexcept
	{
		referenceCount_--;
	}

	static void Reset()
	{
		referenceCount_ = 0;
	}
	static bool IsClear()
	{
		return referenceCount_ == 0;
	}

private:
	static int referenceCount_;
};

int TrivialClass::referenceCount_ = 0;

/**
 * \brief Large object that counts lifetime of its instances
 */
class NonTrivialClass : public TrivialClass
{
private:
	int data_[100] = {};
};

//====================================================================================================
// Types::Any Unit Tests
//====================================================================================================

TEST_CASE("Default Construction", "[any]")
{
	const Types::Any a;
	REQUIRE(a.HasValue() == false);
}

TEST_CASE("Custom Type", "[any]")
{
	struct CustomType
	{
		int data;
	};

	Types::Any a						= CustomType {};
	Types::AnyCast<CustomType&>(a).data = 42;
	REQUIRE(Types::AnyCast<CustomType>(a).data == 42);
}

TEMPLATE_TEST_CASE("Copy and Assignment", "[any]", TrivialClass, NonTrivialClass)
{
	BENCHMARK("std::any")
	{
		std::any stdAny;

		TestType testClass;
		const std::any any1 = testClass;
		const std::any any2(any1);
		std::any any3 = any2;

		// TODO: Enabled when fixed -
		// https://developercommunity.visualstudio.com/t/stdany-doesnt-link-when-exceptions-are-disabled/376072
		// TestType v2 = std::any_cast<TestType&>(any3);
		stdAny = any3;

		return stdAny;
	};

	Types::Any typesAny;
	REQUIRE_FALSE(typesAny.HasValue());

	BENCHMARK("Types::Any")
	{
		TestType testClass;
		const Types::Any any1 = testClass;
		const Types::Any any2(any1);
		Types::Any any3 = any2;

		TestType v2 = Types::AnyCast<TestType&>(any3);
		typesAny	= v2;

		return typesAny;
	};

	REQUIRE(typesAny.HasValue());

	Types::StaticAny typesStaticAny;
	REQUIRE_FALSE(typesStaticAny.HasValue());

	BENCHMARK("Types::StaticAny")
	{
		TestType testClass;
		const Types::StaticAny any1 = testClass;
		const Types::StaticAny any2(any1);
		Types::StaticAny any3 = any2;

		TestType v2 = Types::AnyCast<TestType&>(any3);
		typesAny	= v2;

		return typesAny;
	};

	REQUIRE(typesStaticAny.HasValue());
}

TEMPLATE_TEST_CASE("Class Lifetime", "[any]", TrivialClass, NonTrivialClass)
{
	TestType::Reset();
	{
		Types::Any a {TrivialClass()};
	}
	REQUIRE(TestType::IsClear());
}