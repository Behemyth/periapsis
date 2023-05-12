#pragma once

#include <concepts>
#include <type_traits>

namespace synodic
{
	// Implements the missing concept for 'is_invocable_r'
	template<typename F, typename R, typename... Args>
	concept invocable_r = std::is_invocable_r_v<R, F, Args...>;

	// An alias for 'invocable_r'
	template<typename F, typename R, typename... Args>
	concept callable = invocable_r<F, R, Args...>;

	// Verifies that the given type is the same as its decayed type
	template<typename T>
	concept decayed = std::same_as<T, std::decay_t<T>>;

	// A concept implementation of the named requirement 'Container' with Synodic and Standard naming styles
	//		This definition is devoid of backwards compatibility requirements
	//		https://en.cppreference.com/w/cpp/named_req/Container
	template<typename T>
	concept container =
		requires(T& container, T const& constContainer)
	{
		typename T::value_type;
		typename T::reference;
        typename T::const_reference;

		requires decayed<typename T::value_type>;

		{constContainer.empty()} -> std::convertible_to<bool>;

	};
	// A concept implementation of the named requirement 'SequenceContainer' with Synodic and Standard naming styles
	//		This definition is devoid of backwards compatibility requirements
	//		https://en.cppreference.com/w/cpp/named_req/SequenceContainer
	template <typename T>
    concept sequence_container = container<T> and
		requires(T& cont, T const& const_cont)
	{
		{cont.front()} -> std::same_as<typename T::reference>;
		{const_cont.front()} -> std::same_as<typename T::const_reference>;
	};

}