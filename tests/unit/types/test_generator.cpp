#include <gtest/gtest.h>

import std;
import generator;

namespace
{
	static_assert(std::ranges::input_range<std::generator<int>>);
	static_assert(std::ranges::view<std::generator<int>>);
	static_assert(!std::copyable<std::generator<int>>);
	static_assert(std::same_as<std::ranges::range_reference_t<std::generator<int>>, int&&>);
	static_assert(std::same_as<std::ranges::range_value_t<std::generator<int>>, int>);
	static_assert(
		std::same_as<std::ranges::range_reference_t<std::generator<const std::string&>>, const std::string&>);

	std::generator<int> Count(int limit)
	{
		for (int value = 0; value < limit; ++value)
		{
			co_yield value;
		}
	}

	std::generator<int> Empty()
	{
		co_return;
	}

	std::generator<std::string> Strings()
	{
		co_yield std::string("temporary");

		std::string named = "named";
		co_yield named;
		co_yield std::move(named);
	}

	std::generator<std::unique_ptr<int>> Owned()
	{
		co_yield std::make_unique<int>(1);
		co_yield std::make_unique<int>(2);
	}

	std::generator<const std::string&> References(const std::vector<std::string>& values)
	{
		for (const std::string& value: values)
		{
			co_yield value;
		}
	}

	std::generator<int> Throws()
	{
		co_yield 1;
		throw std::runtime_error("failure");
	}

	struct Guard
	{
		bool& destroyed;

		~Guard()
		{
			destroyed = true;
		}
	};

	std::generator<int> Guarded(bool& destroyed, int& steps)
	{
		Guard guard {destroyed};

		for (int value = 0;; ++value)
		{
			++steps;
			co_yield value;
		}
	}

	std::generator<int> Outer()
	{
		co_yield -1;
		co_yield std::ranges::elements_of(Count(3));
		co_yield std::ranges::elements_of(Empty());
		co_yield 99;
	}

	std::generator<int> Tree(int depth)
	{
		co_yield depth;

		if (depth > 0)
		{
			co_yield std::ranges::elements_of(Tree(depth - 1));
			co_yield std::ranges::elements_of(Tree(depth - 1));
		}
	}

	std::generator<const int&> FromRange()
	{
		const std::vector<int> values {7, 8, 9};

		co_yield 6;
		co_yield std::ranges::elements_of(values);
		co_yield 10;
	}

	std::generator<int> CatchesNested(bool& caught)
	{
		try
		{
			co_yield std::ranges::elements_of(Throws());
		}
		catch (const std::runtime_error&)
		{
			caught = true;
		}

		co_yield 2;
	}

	std::generator<int> PropagatesNested()
	{
		co_yield 0;
		co_yield std::ranges::elements_of(Throws());
		co_yield 5;
	}

	std::generator<int> NestedGuarded(bool& innerDestroyed, bool& outerDestroyed, int& steps)
	{
		Guard guard {outerDestroyed};
		co_yield std::ranges::elements_of(Guarded(innerDestroyed, steps));
	}

	std::generator<std::unique_ptr<int>> NestedOwned()
	{
		co_yield std::ranges::elements_of(Owned());
		co_yield std::make_unique<int>(3);
	}

	template<typename Range>
	std::vector<int> Collect(Range&& range)
	{
		std::vector<int> values;
		for (const int value: range)
		{
			values.push_back(value);
		}
		return values;
	}

	TEST(Generator, YieldsValuesInOrder)
	{
		std::vector<int> values;
		for (const int value: Count(5))
		{
			values.push_back(value);
		}

		EXPECT_EQ(values, (std::vector {0, 1, 2, 3, 4}));
	}

	TEST(Generator, EmptyBodyYieldsNothing)
	{
		auto generator = Empty();
		EXPECT_TRUE(generator.begin() == generator.end());
	}

	TEST(Generator, YieldsTemporariesAndCopiesOfLvalues)
	{
		std::vector<std::string> values;
		for (auto&& value: Strings())
		{
			values.push_back(std::move(value));
		}

		EXPECT_EQ(values, (std::vector<std::string> {"temporary", "named", "named"}));
	}

	TEST(Generator, YieldsMoveOnlyValues)
	{
		int sum = 0;
		for (auto&& pointer: Owned())
		{
			const std::unique_ptr<int> taken = std::move(pointer);
			sum += *taken;
		}

		EXPECT_EQ(sum, 3);
	}

	TEST(Generator, YieldsReferencesWithoutCopying)
	{
		const std::vector<std::string> source {"first", "second"};

		std::size_t index = 0;
		for (const std::string& value: References(source))
		{
			EXPECT_EQ(&value, &source[index]);
			++index;
		}

		EXPECT_EQ(index, source.size());
	}

	TEST(Generator, RunsOnlyAsFarAsItIsConsumed)
	{
		bool destroyed = false;
		int steps = 0;

		{
			auto generator = Guarded(destroyed, steps);
			EXPECT_EQ(steps, 0);

			for (const int value: generator)
			{
				if (value == 1)
				{
					break;
				}
			}

			EXPECT_EQ(steps, 2);
			EXPECT_FALSE(destroyed);
		}

		EXPECT_TRUE(destroyed);
	}

	TEST(Generator, PropagatesExceptionsToTheConsumer)
	{
		auto generator = Throws();
		auto iterator = generator.begin();
		EXPECT_EQ(*iterator, 1);

		EXPECT_THROW(++iterator, std::runtime_error);
	}

	TEST(Generator, WorksWithRangeConversions)
	{
		EXPECT_EQ(std::ranges::to<std::vector<int>>(Count(4)), (std::vector {0, 1, 2, 3}));
	}

	TEST(Generator, MovesOwnershipOfTheCoroutine)
	{
		auto source = Count(3);
		auto target = std::move(source);

		EXPECT_EQ(Collect(target), (std::vector {0, 1, 2}));
	}

	TEST(Generator, YieldsElementsOfNestedGenerators)
	{
		EXPECT_EQ(Collect(Outer()), (std::vector {-1, 0, 1, 2, 99}));
	}

	TEST(Generator, YieldsElementsOfRecursiveGenerators)
	{
		EXPECT_EQ(Collect(Tree(2)), (std::vector {2, 1, 0, 0, 1, 0, 0}));
	}

	TEST(Generator, YieldsElementsOfOtherRanges)
	{
		EXPECT_EQ(Collect(FromRange()), (std::vector {6, 7, 8, 9, 10}));
	}

	TEST(Generator, YieldsMoveOnlyElementsOfNestedGenerators)
	{
		int sum = 0;
		for (auto&& pointer: NestedOwned())
		{
			sum += *pointer;
		}

		EXPECT_EQ(sum, 6);
	}

	TEST(Generator, NestedExceptionCanBeCaughtByTheYieldingGenerator)
	{
		bool caught = false;

		EXPECT_EQ(Collect(CatchesNested(caught)), (std::vector {1, 2}));
		EXPECT_TRUE(caught);
	}

	TEST(Generator, UncaughtNestedExceptionReachesTheConsumer)
	{
		std::vector<int> values;

		EXPECT_THROW(
			{
				for (const int value: PropagatesNested())
				{
					values.push_back(value);
				}
			},
			std::runtime_error);
		EXPECT_EQ(values, (std::vector {0, 1}));
	}

	TEST(Generator, DestroysNestedGeneratorsWithTheOutermost)
	{
		bool innerDestroyed = false;
		bool outerDestroyed = false;
		int steps = 0;

		{
			auto generator = NestedGuarded(innerDestroyed, outerDestroyed, steps);

			for (const int value: generator)
			{
				if (value == 2)
				{
					break;
				}
			}

			EXPECT_EQ(steps, 3);
			EXPECT_FALSE(innerDestroyed);
			EXPECT_FALSE(outerDestroyed);
		}

		EXPECT_TRUE(innerDestroyed);
		EXPECT_TRUE(outerDestroyed);
	}
}
