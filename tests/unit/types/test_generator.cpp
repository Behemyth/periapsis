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

		int total = 0;
		for (const int value: target)
		{
			total += value;
		}

		EXPECT_EQ(total, 3);
	}
}
