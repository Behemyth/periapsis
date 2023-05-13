#pragma once

#include <concepts>
#include <type_traits>

namespace synodic
{
	// Implements the missing concept for 'is_invocable_r'
	template<typename F, typename R, typename... Args>
	concept invocable_r =
		std::is_invocable_r_v<R, F, Args...>;

	// An alias for 'invocable_r'
	template<typename F, typename R, typename... Args>
	concept callable =
		invocable_r<F, R, Args...>;

	// Verifies that the given type is the same as its decayed type
	template<typename T>
	concept decayed =
		std::same_as<T, std::decay_t<T>>;

	// A concept implementation of the named requirement 'Container' with Standard naming styles
	//		This definition is devoid of backwards compatibility requirements
	//		https://en.cppreference.com/w/cpp/named_req/Container
	template<typename T>
	concept container =
		requires(T& container, T const& constContainer)
	{
		typename T::value_type;
		typename T::reference;
        typename T::const_reference;
		typename T::iterator;
        typename T::const_iterator;
		typename T::difference_type;
        typename T::size_type;

		requires decayed<typename T::value_type>;

		{ container.begin() } -> std::same_as<typename T::iterator>;
		{ container.end() } -> std::same_as<typename T::iterator>;
		{ constContainer.begin() } -> std::same_as<typename T::const_iterator>;
		{ constContainer.end() } -> std::same_as<typename T::const_iterator>;
		{ container.cbegin() } -> std::same_as<typename T::const_iterator>;
		{ container.cend() } -> std::same_as<typename T::const_iterator>;

		{ constContainer.max_size() } -> std::same_as<typename T::size_type>;
		{ constContainer.empty() } -> std::convertible_to<bool>;

	};

	// Convenience function for identifying a Container of a specific type
	template <typename T, typename ValueType>
	concept container_of = container<T> and std::same_as<ValueType, typename T::value_type>;

	template <typename T>
    concept reversible_container =
		container<T>;

	// A concept implementation of the named requirement 'SequenceContainer' with Standard naming styles
	//		This definition is devoid of backwards compatibility requirements
	//		https://en.cppreference.com/w/cpp/named_req/SequenceContainer
	template <typename T>
    concept sequence_container =
		container<T> and
		requires(T& container, T const& constContainer)
	{
		{ container.front() } -> std::same_as<typename T::reference>;
		{ constContainer.front() } -> std::same_as<typename T::const_reference>;
	};


	// Convenience function for identifying a SequenceContainer of a specific type
	template <typename T, typename ValueType>
	concept sequence_container_of = container_of<T, ValueType> and sequence_container<T>;


    template <typename T>
    concept double_ended_container =
		sequence_container<T> and
		reversible_container<T> and
		requires(T& container, T const& constContainer)
	{
		{ constContainer.size() } -> std::same_as<typename T::size_type>;


		{ container.back() } -> std::same_as<typename T::reference>;
		{ constContainer.back() } -> std::same_as<typename T::const_reference>;
	};


    template <typename T>
    concept random_access_container =
		double_ended_container<T> and
		requires(T& container, T const& constContainer, typename T::size_type const index)
		{
			{ container[index] } -> std::same_as<typename T::reference>;
			{ constContainer[index] } -> std::same_as<typename T::const_reference>;

			{ container.at(index) } -> std::same_as<typename T::reference>;
			{ constContainer.at(index) } -> std::same_as<typename T::const_reference>;
		};

	// A concept implementation of the named requirement 'ContiguousContainer' with Standard naming styles
	//		This definition is devoid of backwards compatibility requirements
	//		https://en.cppreference.com/w/cpp/named_req/ContiguousContainer
    template <typename T>
    concept contiguous_container =
		random_access_container<T> and
		requires(T& container, T const& constContainer)
    {
		typename T::pointer;
        typename T::const_pointer;

        { container.data() } -> std::same_as<typename T::pointer>;
        { constContainer.data() } -> std::same_as<typename T::const_pointer>;
    };

	// A concept implementation of the named requirement 'AssociativeContainer' with Standard naming styles
	//		This definition is devoid of backwards compatibility requirements
	//		https://en.cppreference.com/w/cpp/named_req/AssociativeContainer
	template <typename T>
    concept associative_container =
		container<T> and
		std::default_initializable<T> and
		requires(T& container, T const& constContainer)
	{
		typename T::key_type;

		container.clear();

		{ constContainer.size() } -> std::same_as<typename T::size_type>;

		requires
			requires(typename T::key_type const& key)
			{
				{ constContainer.count(key) } -> std::same_as<typename T::size_type>;
				{ constContainer.contains(key) } -> std::same_as<bool>;
				{ container.find(key) } -> std::same_as<typename T::iterator>;
			};
	};

}