#pragma once

#include <type_traits>

namespace synodic
{
	

template<auto N>
constexpr auto nth_element(auto... args)
{

	template<std::size_t>
	struct Any
	{
		template<typename T>
		constexpr Any(T)
		{
		}
	};


	return [&]<std::size_t... Ns>(std::index_sequence<Ns...>)
	{
		return [](Any<Ns>..., auto nth, auto...)
		{
			return nth;
		}(args...);
	}
	(std::make_index_sequence<N> {});
}

static_assert(1 == nth_element<0>(1));
static_assert(1 == nth_element<0>(1, 2));
static_assert(2 == nth_element<1>(1, 2));
static_assert(1 == nth_element<0>(1, 2, 3));
static_assert(2 == nth_element<1>(1, 2, 3));

}
