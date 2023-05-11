#pragma once

#include <concepts>
#include <type_traits>

namespace synodic
{

	template<typename R, typename F, typename... Args>
	concept invocable_r = std::is_invocable_r_v<R, F, Args...>;

	// An alias for 'invocable_r'
	template<typename R, typename F, typename... Args>
	concept callable = invocable_r<R, F, Args...>;

	template<typename T>
	concept decayed = std::same_as<T, std::decay_t<T>>;

	template<typename C>
	concept container =
		requires(C& cont, const C& const_cont) {
			typename C::value_type;
			requires decayed<typename C::value_type>;

			typename C::reference;
			typename C::const_reference;
			// Well-behaved containers should not rely on proxies for iterators / references.
			requires std::same_as<typename C::reference, typename C::value_type&>;
			requires std::same_as<typename C::const_reference, typename C::value_type const&>;

			typename C::iterator;
			typename C::const_iterator;
			requires std::forward_iterator<typename C::iterator>;
			requires std::forward_iterator<typename C::const_iterator>;
			requires std::convertible_to<typename C::iterator, typename C::const_iterator>;
			requires std::same_as<std::iter_value_t<typename C::iterator>, typename C::value_type>;
			requires std::same_as<std::iter_value_t<typename C::const_iterator>, typename C::value_type>;
			requires std::same_as<std::iter_reference_t<typename C::iterator>, typename C::reference> or
						 std::same_as<std::iter_reference_t<typename C::iterator>, typename C::const_reference>;
			requires std::same_as<std::iter_reference_t<typename C::const_iterator>, typename C::const_reference>;

			typename C::difference_type;
			typename C::size_type;
			requires std::signed_integral<typename C::difference_type>;
			requires std::unsigned_integral<typename C::size_type>;
			// size_type should be able to represent all positive values of difference_type.
			requires std::in_range<typename C::size_type>(std::numeric_limits<typename C::difference_type>::max());
			// difference_type should be the same as the one defined by iterator_traits.
			requires std::same_as<
				typename C::difference_type,
				typename std::iterator_traits<typename C::iterator>::difference_type>;
			requires std::same_as<
				typename C::difference_type,
				typename std::iterator_traits<typename C::const_iterator>::difference_type>;

			// Containers of equality comparable values should be equality comparable
			requires not std::equality_comparable<typename C::value_type> or std::equality_comparable<C>;

			// Containers should satisfy at least the strongest object concept satisfied by value_type.
			requires not std::movable<typename C::value_type> or std::movable<C>;
			requires not std::copyable<typename C::value_type> or std::copyable<C>;
			requires not std::semiregular<typename C::value_type> or std::semiregular<C>;
			requires not std::regular<typename C::value_type> or std::regular<C>;

			// Iterators
			{
				cont.begin()
				} -> std::same_as<typename C::iterator>;
			{
				cont.end()
				} -> std::same_as<typename C::iterator>;
			{
				const_cont.begin()
				} -> std::same_as<typename C::const_iterator>;
			{
				const_cont.end()
				} -> std::same_as<typename C::const_iterator>;
			{
				cont.cbegin()
				} -> std::same_as<typename C::const_iterator>;
			{
				cont.cend()
				} -> std::same_as<typename C::const_iterator>;

			// Capacity
			{
				const_cont.max_size()
				} -> std::same_as<typename C::size_type>;
			{
				const_cont.empty()
				} -> std::convertible_to<bool>;
		};

}