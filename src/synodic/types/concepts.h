// Container concepts inspired by https://github.com/MiSo1289/more_concepts

/* MIT License

Copyright (c) 2021 Michal Jankovič

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/


#pragma once

#include <concepts>
#include <type_traits>
#include <numeric>
#include <iterator>

namespace synodic
{
	// Implements the missing concept for 'is_invocable_r'
	template<typename Fn, typename R, typename... Args>
	concept invocable_r =
		std::is_invocable_r_v<R, Fn, Args...>;

	// An alias for 'invocable_r'
	template<typename Fn, typename R, typename... Args>
	concept callable =
		invocable_r<Fn, R, Args...>;

	// Verifies that the given type is the same as its decayed type
	template<typename T>
	concept decayed =
		std::same_as<T, std::decay_t<T>>;


	// A concept implementation of the named requirement 'Hash' with Standard naming styles
	//		This definition is devoid of backwards compatibility requirements
	//		https://en.cppreference.com/w/cpp/named_req/Hash
	template <typename Fn, typename KeyType>
	concept hash_function = callable<Fn const, auto(KeyType&) -> std::size_t, auto(KeyType const&) -> std::size_t>;

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

		requires
			decayed<typename T::value_type>;

		// Well-behaved containers should not rely on proxies for iterators / references.
		requires std::same_as<typename T::reference, typename T::value_type&>;
        requires std::same_as<typename T::const_reference, typename T::value_type const&>;

		requires std::forward_iterator<typename T::iterator>;
		requires std::forward_iterator<typename T::const_iterator>;
		requires std::convertible_to<typename T::iterator, typename T::const_iterator>;
		requires std::same_as<std::iter_value_t<typename T::iterator>, typename T::value_type>;
		requires std::same_as<std::iter_value_t<typename T::const_iterator>, typename T::value_type>;
		requires std::same_as<std::iter_reference_t<typename T::iterator>, typename T::reference> or
		         std::same_as<std::iter_reference_t<typename T::iterator>, typename T::const_reference>;
		requires std::same_as<std::iter_reference_t<typename T::const_iterator>, typename T::const_reference>;

		requires std::signed_integral<typename T::difference_type>;
        requires std::unsigned_integral<typename T::size_type>;

        // size_type should be able to represent all positive values of difference_type.
        requires std::in_range<typename T::size_type>(
            std::numeric_limits<typename T::difference_type>::max());

        // difference_type should be the same as the one defined by iterator_traits.
        requires std::same_as<
            typename T::difference_type,
            typename std::iterator_traits<typename T::iterator>::difference_type>;
        requires std::same_as<
            typename T::difference_type,
            typename std::iterator_traits<typename T::const_iterator>::difference_type>;

		requires
			not std::equality_comparable<typename T::value_type> or
			std::equality_comparable<T>;

		requires
			not std::movable<typename T::value_type> or
            std::movable<T>;
        requires
			not std::copyable<typename T::value_type> or
			 std::copyable<T>;
        requires
			not std::semiregular<typename T::value_type> or
            std::semiregular<T>;
        requires
			not std::regular<typename T::value_type> or
            std::regular<T>;

		{ container.begin() } -> std::same_as<typename T::iterator>;
		{ container.end() } -> std::same_as<typename T::iterator>;
		{ constContainer.begin() } -> std::same_as<typename T::const_iterator>;
		{ constContainer.end() } -> std::same_as<typename T::const_iterator>;
		{ container.cbegin() } -> std::same_as<typename T::const_iterator>;
		{ container.cend() } -> std::same_as<typename T::const_iterator>;

		{ constContainer.max_size() } -> std::same_as<typename T::size_type>;
		{ constContainer.empty() } -> std::convertible_to<bool>;

	};

	// Convenience concept for identifying a container of a specific type
	template <typename T, typename ValueType>
	concept container_of = container<T> and std::same_as<ValueType, typename T::value_type>;

	template <typename T>
	concept mutable_container = container<T> and
		std::same_as<std::iter_reference_t<typename T::iterator>, typename T::reference>;

	// Convenience concept for identifying a mutable container of a specific type
	template <typename T, typename ValueType>
    concept mutable_container_of = container_of<T, ValueType> and mutable_container<T>;

	template <typename T>
	concept sized_container = container<T> and
		requires(T const& constContainer)
	{
		{ constContainer.size() } -> std::same_as<typename T::size_type>;
	};

	// Convenience concept for identifying a sized container of a specific type
	template <typename T, typename ValueType>
    concept sized_container_of = container_of<T, ValueType> and sized_container<T>;

	template <typename T>
	concept clearable_container = container<T> and
		requires(T& container)
	{
		container.clear();
	};

	// Convenience concept for identifying a clearable container of a specific type
	template <typename T, typename ValueType>
    concept clearable_container_of = container_of<T, ValueType> and clearable_container<T>;

	template <typename T>
    concept reversible_container
    = container<T> and
      requires(T& container, T const& constContainer)
      {
          requires std::bidirectional_iterator<typename T::iterator>;
          requires std::bidirectional_iterator<typename T::const_iterator>;

          typename T::reverse_iterator;
          typename T::const_reverse_iterator;
          requires std::bidirectional_iterator<typename T::reverse_iterator>;
          requires std::bidirectional_iterator<typename T::const_reverse_iterator>;
          requires std::convertible_to<
              typename T::reverse_iterator,
              typename T::const_reverse_iterator>;
          requires std::same_as<
              typename T::difference_type,
              typename std::iterator_traits<typename T::reverse_iterator>::difference_type>;
          requires std::same_as<
              typename T::difference_type,
              typename std::iterator_traits<typename T::const_reverse_iterator>::difference_type>;

          { container.rbegin() } -> std::same_as<typename T::reverse_iterator>;
          { container.rend() } -> std::same_as<typename T::reverse_iterator>;
          { constContainer.rbegin() } -> std::same_as<typename T::const_reverse_iterator>;
          { constContainer.rend() } -> std::same_as<typename T::const_reverse_iterator>;
          { container.crbegin() } -> std::same_as<typename T::const_reverse_iterator>;
          { container.crend() } -> std::same_as<typename T::const_reverse_iterator>;
      };


	// Convenience concept for identifying a reversable container of a specific type
	template <typename T, typename ValueType>
    concept reversible_container_of = container_of<T, ValueType> and reversible_container<T>;

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

		requires
			not std::totally_ordered<typename T::value_type> or
            std::totally_ordered<T>;
	};


	// Convenience function for identifying a SequenceContainer of a specific type
	template <typename T, typename ValueType>
	concept sequence_container_of = container_of<T, ValueType> and sequence_container<T>;


    template <typename T>
    concept double_ended_container =
		sequence_container<T> and
		sized_container<T> and
		reversible_container<T> and
		requires(T& container, T const& constContainer)
	{
		{ container.back() } -> std::same_as<typename T::reference>;
		{ constContainer.back() } -> std::same_as<typename T::const_reference>;
	};


    template <typename T>
    concept random_access_container =
		double_ended_container<T> and
		requires(T& container, T const& constContainer, typename T::size_type const index)
		{
			requires std::random_access_iterator<typename T::iterator>;
			requires std::random_access_iterator<typename T::const_iterator>;

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

		requires std::contiguous_iterator<typename T::iterator>;
        requires std::contiguous_iterator<typename T::const_iterator>;
		requires std::convertible_to<typename T::pointer, typename T::const_pointer>;

        { container.data() } -> std::same_as<typename T::pointer>;
        { constContainer.data() } -> std::same_as<typename T::const_pointer>;
    };

	template <typename T>
	concept resizable_sequence_container =
		double_ended_container<T> and
		clearable_container<T> and
		std::default_initializable<T> and
		requires(T& container, typename T::const_iterator const& position)
	{
		// Copy-enabled operations
		requires not std::copyable<typename T::value_type> or
		       requires(
		       typename T::size_type const& size,
		       typename T::value_type const& value,
		       std::iter_const_reference_t<typename T::value_type> const& first,
		       std::iter_const_reference_t<typename T::value_type> const& last,
		       std::initializer_list<typename T::value_type> const& init_list)
		       {
		           // Copy insertion
		           { container.insert(position, value) } -> std::same_as<typename T::iterator>;
		           { container.insert(position, first, last) } -> std::same_as<typename T::iterator>;
		           { container.insert(position, init_list) } -> std::same_as<typename T::iterator>;

		           // Range construction and assignment
		           T(first, last);
		           T(init_list);
		           T(size, value);
		           container = init_list;
		           container.assign(first, last);
		           container.assign(init_list);
		           container.assign(size, value);

		           // Resizing
		           container.resize(size);
		           container.resize(size, value);
		       };

		// Move insertion
		requires not std::movable<typename T::value_type> or
		       requires(typename T::value_type&& value)
		       {{ container.insert(position, std::move(value)) } -> std::same_as<typename T::iterator>; };

		// Erasure
		requires
		requires(
		  typename T::const_iterator const& first,
		  typename T::const_iterator const& last)
		{
		  { container.erase(position) } -> std::same_as<typename T::iterator>;
		  { container.erase(first, last) } -> std::same_as<typename T::iterator>;
		};
	};

    /// A resizable container that also allows in-place element construction.
    /// Satisfied by vector, deque and list.
    ///
    /// Note: the container is required to support .emplace(const_iterator, Args...)
    /// for every constructor of the form value_type(Args...).
    template <typename T>
    concept inplace_constructing_sequence_container
    = resizable_sequence_container<T> and
      requires(T& container, typename T::const_iterator const& pos)
      {
          // Emplacement (default constructor)
          requires not std::default_initializable<typename T::value_type> or
                   requires {{ container.emplace(pos) } -> std::same_as<typename T::iterator>; };

          // Emplacement (copy constructor)
          requires not std::copyable<typename T::value_type> or
                   requires(typename T::value_type const& value)
                   {{ container.emplace(pos, value) } -> std::same_as<typename T::iterator>; };

          // Emplacement (move constructor)
          requires not std::movable<typename T::value_type> or
                   requires(typename T::value_type&& value)
                   {{ container.emplace(pos, std::move(value)) } -> std::same_as<typename T::iterator>; };
      };

    /// A sequence container that allows efficient insertion and erasure at the front.
    /// Satisfied by deque, list and forward_list.
    template <typename T>
    concept front_growable_container
    = sequence_container<T> and
      std::default_initializable<T> and
      requires(T& container, typename T::value_type const& value, typename T::value_type&& tmp_value)
      {
          // Front copy insertion
          requires not std::copyable<typename T::value_type> or
                   requires { container.push_front(value); };

          // Front move insertion
          requires not std::movable<typename T::value_type> or
                   requires { container.push_front(std::move(tmp_value)); };

          // Front erasure
          container.pop_front();
      };

    /// A front-growable container that also allows inplace front construction.
    /// Satisfied by deque, list and forward_list.
    ///
    /// Note: the container is required to support .emplace_front(Args...)
    /// for every constructor of the form value_type(Args...).
    template <typename T>
    concept inplace_front_constructing_container
    = front_growable_container<T> and
      requires(T& container, typename T::value_type const& value, typename T::value_type&& tmp_value)
      {
          // Front emplacement (default constructor)
          requires not std::default_initializable<typename T::value_type> or
                   requires {{ container.emplace_front() } -> std::same_as<typename T::reference>; };

          // Front emplacement (copy constructor)
          requires not std::copyable<typename T::value_type> or
                   requires {{ container.emplace_front(value) } -> std::same_as<typename T::reference>; };

          // Front emplacement (move constructor)
          requires not std::movable<typename T::value_type> or
                   requires {{ container.emplace_front(std::move(tmp_value)) } -> std::same_as<typename T::reference>; };
      };

    /// A double-ended container that allows efficient insertion and erasure at the back.
    /// Satisfied by vector, basic_string, deque, and list.
    template <typename T>
    concept back_growable_container
    = double_ended_container<T> and
      std::default_initializable<T> and
      requires(T& container, typename T::value_type const& value, typename T::value_type&& tmp_value)
      {
          // Back copy insertion
          requires not std::copyable<typename T::value_type> or
                   requires { container.push_back(value); };

          // Back move insertion
          requires not std::movable<typename T::value_type> or
                   requires { container.push_back(std::move(tmp_value)); };

          // Back erasure
          container.pop_back();
      };

    /// A back-growable container that also allows inplace back construction.
    /// Satisfied by vector, deque, and list.
    ///
    /// Note: the container is required to support .emplace_back(Args...)
    /// for every constructor of the form value_type(Args...).
    template <typename T>
    concept inplace_back_constructing_container
    = back_growable_container<T> and
      requires(T& container, typename T::value_type const& value, typename T::value_type&& tmp_value)
      {
          // Back emplacement (default constructor)
          requires not std::default_initializable<typename T::value_type> or
                   requires {{ container.emplace_back() } -> std::same_as<typename T::reference>; };

          // Back emplacement (copy constructor)
          requires not std::copyable<typename T::value_type> or
                   requires {{ container.emplace_back(value) } -> std::same_as<typename T::reference>; };

          // Back emplacement (move constructor)
          requires not std::movable<typename T::value_type> or
                   requires {{ container.emplace_back(std::move(tmp_value)) } -> std::same_as<typename T::reference>; };
      };

    template <typename T, typename ValueType>
    concept double_ended_container_of = container_of<T, ValueType> and double_ended_container<T>;

    template <typename T, typename ValueType>
    concept random_access_container_of = container_of<T, ValueType> and random_access_container<T>;

    template <typename T, typename ValueType>
    concept contiguous_container_of = container_of<T, ValueType> and contiguous_container<T>;

    template <typename T, typename ValueType>
    concept resizable_sequence_container_of = container_of<T, ValueType> and resizable_sequence_container<T>;

    template <typename T, typename ValueType>
    concept inplace_constructing_sequence_container_of
    = container_of<T, ValueType> and inplace_back_constructing_container<T>;

    template <typename T, typename ValueType>
    concept front_growable_container_of = container_of<T, ValueType> and front_growable_container<T>;

    template <typename T, typename ValueType>
    concept inplace_front_constructing_container_of
    = container_of<T, ValueType> and inplace_front_constructing_container<T>;

    template <typename T, typename ValueType>
    concept back_growable_container_of = container_of<T, ValueType> and back_growable_container<T>;

    template <typename T, typename ValueType>
    concept inplace_back_constructing_container_of
    = container_of<T, ValueType> and inplace_back_constructing_container<T>;


	// A concept implementation of the named requirement 'AssociativeContainer' with Standard naming styles
	//		This definition is devoid of backwards compatibility requirements
	//		https://en.cppreference.com/w/cpp/named_req/AssociativeContainer
	template <typename C>
    concept associative_container
    = container<C> and
      sized_container<C> and
      clearable_container<C> and
      std::default_initializable<C> and
      requires(C& container, C const& const_cont)
      {
          typename C::key_type;

          // Searching
          requires
          requires(typename C::key_type const& key)
          {
              { const_cont.count(key) } -> std::same_as<typename C::size_type>;
              { const_cont.contains(key) } -> std::same_as<bool>;
              { container.find(key) } -> std::same_as<typename C::iterator>;
              { const_cont.find(key) } -> std::same_as<typename C::const_iterator>;
              {
              container.equal_range(key)
              } -> std::same_as<std::pair<typename C::iterator, typename C::iterator>>;
              {
              const_cont.equal_range(key)
              } -> std::same_as<std::pair<typename C::const_iterator, typename C::const_iterator>>;
          };

          // Emplacement (default constructor)
          requires not std::default_initializable<typename C::value_type> or
                   requires(typename C::const_iterator const& hint)
                   {{ container.emplace_hint(hint) } -> std::same_as<typename C::iterator>; };

          // Copy-enabled operations
          requires not std::copyable<typename C::value_type> or
                   requires(
                       typename C::value_type const& value,
                       typename C::const_iterator const& hint,
                       std::iter_const_reference_t<typename C::value_type> const& first,
                       std::iter_const_reference_t<typename C::value_type> const& last,
                       std::initializer_list<typename C::value_type> const& init_list)
                   {
                       // Range construction and assignment
                       C(first, last);
                       C(init_list);
                       container = init_list;

                       // Copy insertion
                       { container.insert(hint, value) } -> std::same_as<typename C::iterator>;

                       // Range insertion
                       container.insert(first, last);
                       container.insert(init_list);

                       // Emplacement (copy constructor)
                       { container.emplace_hint(hint, value) } -> std::same_as<typename C::iterator>;
                   };

          // Move-enabled operations
          requires not std::movable<typename C::value_type> or
                   requires(
                       typename C::value_type&& tmp_value,
                       typename C::const_iterator const& hint)
                   {
                       // Move insertion
                       { container.insert(hint, std::move(tmp_value)) } -> std::same_as<typename C::iterator>;

                       // Emplacement (move constructor)
                       { container.emplace_hint(hint, std::move(tmp_value)) } -> std::same_as<typename C::iterator>;
                   };

          // Erasure
          requires
          requires(
              typename C::const_iterator const& pos,
              typename C::const_iterator const& first,
              typename C::const_iterator const& last,
              typename C::key_type const& key)
          {
              { container.erase(pos) } -> std::same_as<typename C::iterator>;
              { container.erase(first, last) } -> std::same_as<typename C::iterator>;
              { container.erase(key) } -> std::same_as<typename C::size_type>;
          };
      };

    /// An associative container without multiple key occurrence.
    /// Satisfied by set, map, unordered_set and unordered_map.
    ///
    /// Note: the container is required to support .emplace(Args...) for every constructor
    /// of the form value_type(Args...).
    template <typename C>
    concept unique_associative_container
    = associative_container<C> and
      requires(C& container, C const& const_cont)
      {
          // Emplacement (default constructor)
          requires not std::default_initializable<typename C::value_type> or
                   requires(typename C::const_iterator const& hint)
                   {{ container.emplace() } -> std::same_as<std::pair<typename C::iterator, bool>>; };

          // Copy-enabled operations
          requires not std::copyable<typename C::value_type> or
                   requires(typename C::value_type const& value)
                   {
                       // Copy insertion
                       { container.insert(value) } -> std::same_as<std::pair<typename C::iterator, bool>>;

                       // Emplacement (copy constructor)
                       { container.emplace(value) } -> std::same_as<std::pair<typename C::iterator, bool>>;
                   };

          // Move-enabled operations
          requires not std::movable<typename C::value_type> or
                   requires(typename C::value_type&& tmp_value)
                   {
                       // Move insertion
                       { container.insert(std::move(tmp_value)) } -> std::same_as<std::pair<typename C::iterator, bool>>;

                       // Emplacement (move constructor)
                       { container.emplace(std::move(tmp_value)) } -> std::same_as<std::pair<typename C::iterator, bool>>;
                   };
      };

    /// An associative container with multiple key occurrence.
    /// Satisfied by multiset, multimap, unordered_multiset and unordered_multimap.
    ///
    /// Note: the container is required to support .emplace(Args...) for every constructor
    /// of the form value_type(Args...).
    template <typename C>
    concept multiple_associative_container
    = associative_container<C> and
      requires(C& container, C const& const_cont)
      {
          // Emplacement (default constructor)
          requires not std::default_initializable<typename C::value_type> or
                   requires(typename C::const_iterator const& hint)
                   {{ container.emplace() } -> std::same_as<typename C::iterator>; };

          // Copy-enabled operations
          requires not std::copyable<typename C::value_type> or
                   requires(typename C::value_type const& value)
                   {
                       // Copy insertion
                       { container.insert(value) } -> std::same_as<typename C::iterator>;

                       // Emplacement (copy constructor)
                       { container.emplace(value) } -> std::same_as<typename C::iterator>;
                   };

          // Move-enabled operations
          requires not std::movable<typename C::value_type> or
                   requires(typename C::value_type&& tmp_value)
                   {
                       // Move insertion
                       { container.insert(std::move(tmp_value)) } -> std::same_as<typename C::iterator>;

                       // Emplacement (move constructor)
                       { container.emplace(std::move(tmp_value)) } -> std::same_as<typename C::iterator>;
                   };
      };

    /// An associative container ordered on keys.
    /// Satisfied by set, map, multiset and multimap.
    ///
    /// Note: corresponds to the standard AssociativeContainer named requirement.
    template <typename C>
    concept ordered_associative_container
    = associative_container<C> and
      requires(C& container, C const& const_cont)
      {
          // Comparators
          typename C::key_compare;
          typename C::value_compare;
          requires std::strict_weak_order<
              typename C::key_compare, typename C::key_type, typename C::key_type>;
          requires std::strict_weak_order<
              typename C::value_compare, typename C::value_type, typename C::value_type>;

          // Ordered associative containers of ordered values should provide a lexicographical ordering
          requires not std::totally_ordered<typename C::value_type> or
                   std::totally_ordered<C>;

          // Observers
          { const_cont.key_comp() } -> std::same_as<typename C::key_compare>;
          { const_cont.value_comp() } -> std::same_as<typename C::value_compare>;

          // Construction with comparator
          requires
          requires(typename C::key_compare const& key_comp)
          {
              C(key_comp);

              requires not std::copyable<typename C::value_type> or
                       requires(
                            std::iter_const_reference_t<typename C::value_type> const& first,
                            std::iter_const_reference_t<typename C::value_type> const& last,
                           std::initializer_list<typename C::value_type> const& init_list)
                       {
                           C(first, last, key_comp);
                           C(init_list, key_comp);
                       };
          };

          // Binary searching
          requires
          requires(typename C::key_type const& key)
          {
              { container.lower_bound(key) } -> std::same_as<typename C::iterator>;
              { const_cont.lower_bound(key) } -> std::same_as<typename C::const_iterator>;
              { container.upper_bound(key) } -> std::same_as<typename C::iterator>;
              { const_cont.upper_bound(key) } -> std::same_as<typename C::const_iterator>;
          };
      };

    /// An ordered associative container without multiple key occurrence.
    /// Satisfied by set and map.
    template <typename C>
    concept ordered_unique_associative_container
    = unique_associative_container<C> and ordered_associative_container<C>;

    /// An ordered associative container with multiple key occurrence.
    /// Satisfied by multiset and multimap.
    template <typename C>
    concept ordered_multiple_associative_container
    = multiple_associative_container<C> and ordered_associative_container<C>;

    /// An associative container based on key hashing (hash table).
    /// Satisfied by unordered_set, unordered_map, unordered_multiset and unordered_multimap.
    ///
    /// Note: corresponds to the standard UnorderedAssociativeContainer named requirement.
    template <typename C>
    concept unordered_associative_container
    = associative_container<C> and
      requires(C& container, C const& const_cont)
      {
          // Key hash function
          typename C::hasher;
          requires hash_function<typename C::hasher, typename C::key_type>;

          // Key equality predicate
          typename C::key_equal;
          requires std::equivalence_relation<
              typename C::key_equal, typename C::key_type, typename C::key_type>;

          // Observers
          { const_cont.hash_function() } -> std::same_as<typename C::hasher>;
          { const_cont.key_eq() } -> std::same_as<typename C::key_equal>;

          // Hash policy
          requires
          requires(float const& ml, std::size_t n) {
              { const_cont.load_factor() } -> std::same_as<float>;
              { const_cont.max_load_factor() } -> std::same_as<float>;
              container.max_load_factor(ml);
              container.rehash(n);
              container.reserve(n);
          };

          // Minimal bucket interface
          { const_cont.bucket_count() } -> std::same_as<std::size_t>;
          // Not requiring local_iterator etc as its not provided by many popular
          // non-std hash tables, e.g. absl::flat_hash_set and tsl::robin_map.

          // Construction with bucket count, hash function, and equality predicate
          requires
          requires(
              std::size_t const& bucket_count,
              typename C::hasher const& hash,
              typename C::key_equal const& equal)
          {
              C(bucket_count);
              C(bucket_count, hash);
              C(bucket_count, hash, equal);

              requires not std::copyable<typename C::value_type> or
                       requires(
                            std::iter_const_reference_t<typename C::value_type> const& first,
                            std::iter_const_reference_t<typename C::value_type> const& last,
                           std::initializer_list<typename C::value_type> const& init_list)
                       {
                           C(first, last, bucket_count);
                           C(first, last, bucket_count, hash);
                           C(first, last, bucket_count, hash, equal);
                           C(init_list, bucket_count);
                           C(init_list, bucket_count, hash);
                           C(init_list, bucket_count, hash, equal);
                       };
          };
      };

    /// A hash-based associative container without multiple key occurrence.
    /// Satisfied by unordered_set and unordered_map.
    template <typename C>
    concept unordered_unique_associative_container
    = unique_associative_container<C> and unordered_associative_container<C>;

    /// A hash-based associative container with multiple key occurrence.
    /// Satisfied by unordered_multiset and unordered_multimap.
    template <typename C>
    concept unordered_multiple_associative_container
    = multiple_associative_container<C> and unordered_associative_container<C>;

    /// An associative container that stores a mapped object alongside its keys.
    /// Satisfied by map, multimap, unordered_map and unordered_multimap.
    template <typename C>
    concept map_container
    = associative_container<C> and
      requires(C& container, C const& const_cont)
      {
          typename C::mapped_type;

          requires
          requires(typename C::const_iterator const& hint)
          {
              requires not std::copyable<typename C::key_type> or
                       requires(typename C::key_type const& key)
                       {
                           // Emplacement (key copy, mapped object default constructor)
                           requires not std::default_initializable<typename C::mapped_type> or
                                    requires
                                    {
                                        {
                                        container.emplace_hint(hint, key)
                                        } -> std::same_as<typename C::iterator>;
                                        {
                                        container.emplace_hint(
                                            hint,
                                            std::piecewise_construct,
                                            std::forward_as_tuple(key),
                                            std::forward_as_tuple())
                                        }  -> std::same_as<typename C::iterator>;
                                    };

                           // Emplacement (key copy, mapped object copy constructor)
                           requires not std::copyable<typename C::mapped_type> or
                                    requires(typename C::mapped_type const& obj)
                                    {
                                        {
                                        container.emplace_hint(hint, key, obj)
                                        } -> std::same_as<typename C::iterator>;
                                        {
                                        container.emplace_hint(
                                            hint,
                                            std::piecewise_construct,
                                            std::forward_as_tuple(key),
                                            std::forward_as_tuple(obj))
                                        }  -> std::same_as<typename C::iterator>;
                                    };

                           // Emplacement (key copy, mapped object move constructor)
                           requires not std::movable<typename C::mapped_type> or
                                    requires(typename C::mapped_type&& obj)
                                    {
                                        {
                                        container.emplace_hint(hint, key, std::move(obj))
                                        } -> std::same_as<typename C::iterator>;
                                        {
                                        container.emplace_hint(
                                            hint,
                                            std::piecewise_construct,
                                            std::forward_as_tuple(key),
                                            std::forward_as_tuple(std::move(obj)))
                                        }  -> std::same_as<typename C::iterator>;
                                    };
                       };

              requires not std::movable<typename C::key_type> or
                       requires(typename C::key_type&& key)
                       {
                           // Emplacement (key move, mapped object default constructor)
                           requires not std::default_initializable<typename C::mapped_type> or
                                    requires
                                    {
                                        {
                                        container.emplace_hint(hint, std::move(key))
                                        } -> std::same_as<typename C::iterator>;
                                        {
                                        container.emplace_hint(
                                            hint,
                                            std::piecewise_construct,
                                            std::forward_as_tuple(std::move(key)),
                                            std::forward_as_tuple())
                                        }  -> std::same_as<typename C::iterator>;
                                    };

                           // Emplacement (key move, mapped object copy constructor)
                           requires not std::copyable<typename C::mapped_type> or
                                    requires(typename C::mapped_type const& obj)
                                    {
                                        {
                                        container.emplace_hint(hint, std::move(key), obj)
                                        } -> std::same_as<typename C::iterator>;
                                        {
                                        container.emplace_hint(
                                            hint,
                                            std::piecewise_construct,
                                            std::forward_as_tuple(std::move(key)),
                                            std::forward_as_tuple(obj))
                                        }  -> std::same_as<typename C::iterator>;
                                    };

                           // Emplacement (key move, mapped object move constructor)
                           requires not std::movable<typename C::mapped_type> or
                                    requires(typename C::mapped_type&& obj)
                                    {
                                        {
                                        container.emplace_hint(hint, std::move(key), std::move(obj))
                                        } -> std::same_as<typename C::iterator>;
                                        {
                                        container.emplace_hint(
                                            hint,
                                            std::piecewise_construct,
                                            std::forward_as_tuple(std::move(key)),
                                            std::forward_as_tuple(std::move(obj)))
                                        }  -> std::same_as<typename C::iterator>;
                                    };
                       };
          };
      };

    /// A map container without multiple key occurrence.
    /// Satisfied by map and unordered_map.
    ///
    /// Note: the container is required to support
    /// .try_emplace(key_type const&, Args...)
    /// .try_emplace(key_type&&, Args...)
    /// .try_emplace(const_iterator, key_type const&, Args...)
    /// .try_emplace(const_iterator, key_type&&, Args...)
    /// for every constructor of the form mapped_type(Args...), and
    /// .insert_or_assign(key_type const&, M&&)
    /// .insert_or_assign(key_type&&, M&&)
    /// .insert_or_assign(const_iterator, key_type const&, M&&)
    /// .insert_or_assign(const_iterator, key_type&&, M&&)
    /// for every type M where std::is_assignable_v<mapped_type&, M&&> is true.
    template <typename C>
    concept unique_map_container
    = map_container<C> and
      requires(C& container, C const& const_cont)
      {
          // Element access
          requires
          requires(typename C::key_type const& key, typename C::key_type const& tmp_key)
          {
              // operator[] access
              requires not std::default_initializable<typename C::mapped_type> or
                       requires
                       {
                           // Key copy
                           requires not std::copyable<typename C::key_type> or
                                    requires {{ container[key] } -> std::same_as<typename C::mapped_type&>; };

                           // Key move
                           requires not std::movable<typename C::key_type> or
                                    requires
                                    {{ container[std::move(tmp_key)] } -> std::same_as<typename C::mapped_type&>; };
                       };

              // Checked access
              { container.at(key) } -> std::same_as<typename C::mapped_type&>;
              { const_cont.at(key) } -> std::same_as<typename C::mapped_type const&>;
          };

          requires
          requires(typename C::const_iterator const& hint)
          {
              // Key copy
              requires not std::copyable<typename C::key_type> or
                       requires(typename C::key_type const& key)
                       {
                           // Emplacement (key copy, mapped object default constructor)
                           requires not std::default_initializable<typename C::mapped_type> or
                                    requires
                                    {
                                        {
                                        container.emplace(key)
                                        } -> std::same_as<std::pair<typename C::iterator, bool>>;
                                        {
                                        container.emplace(
                                            std::piecewise_construct,
                                            std::forward_as_tuple(key),
                                            std::forward_as_tuple())
                                        } -> std::same_as<std::pair<typename C::iterator, bool>>;
                                        {
                                        container.try_emplace(key)
                                        } -> std::same_as<std::pair<typename C::iterator, bool>>;
                                        {
                                        container.try_emplace(hint, key)
                                        } -> std::same_as<typename C::iterator>;
                                    };

                           // Mapped object copy
                           requires not std::copyable<typename C::mapped_type> or
                                    requires(typename C::mapped_type const& obj)
                                    {
                                        // Insertion
                                        {
                                        container.insert_or_assign(key, obj)
                                        } -> std::same_as<std::pair<typename C::iterator, bool>>;
                                        {
                                        container.insert_or_assign(hint, key, obj)
                                        } -> std::same_as<typename C::iterator>;

                                        // Emplacement (key copy, mapped object copy constructor)
                                        {
                                        container.emplace(key, obj)
                                        } -> std::same_as<std::pair<typename C::iterator, bool>>;
                                        {
                                        container.emplace(
                                            std::piecewise_construct,
                                            std::forward_as_tuple(key),
                                            std::forward_as_tuple(obj))
                                        } -> std::same_as<std::pair<typename C::iterator, bool>>;
                                        {
                                        container.try_emplace(key, obj)
                                        } -> std::same_as<std::pair<typename C::iterator, bool>>;
                                        {
                                        container.try_emplace(hint, key, obj)
                                        } -> std::same_as<typename C::iterator>;
                                    };

                           // Mapped object move
                           requires not std::movable<typename C::mapped_type> or
                                    requires(typename C::mapped_type&& obj)
                                    {
                                        // Insertion
                                        {
                                        container.insert_or_assign(key, std::move(obj))
                                        } -> std::same_as<std::pair<typename C::iterator, bool>>;
                                        {
                                        container.insert_or_assign(hint, key, std::move(obj))
                                        } -> std::same_as<typename C::iterator>;

                                        // Emplacement (key copy, mapped object move constructor)
                                        {
                                        container.emplace(key, std::move(obj))
                                        } -> std::same_as<std::pair<typename C::iterator, bool>>;
                                        {
                                        container.emplace(
                                            std::piecewise_construct,
                                            std::forward_as_tuple(key),
                                            std::forward_as_tuple(std::move(obj)))
                                        } -> std::same_as<std::pair<typename C::iterator, bool>>;
                                        {
                                        container.try_emplace(key, std::move(obj))
                                        } -> std::same_as<std::pair<typename C::iterator, bool>>;
                                        {
                                        container.try_emplace(hint, key, std::move(obj))
                                        } -> std::same_as<typename C::iterator>;
                                    };
                       };

              // Key move
              requires not std::movable<typename C::key_type> or
                       requires(typename C::key_type&& key)
                       {
                           // Emplacement (key move, mapped object default constructor)
                           requires not std::default_initializable<typename C::mapped_type> or
                                    requires
                                    {
                                        {
                                        container.emplace(std::move(key))
                                        } -> std::same_as<std::pair<typename C::iterator, bool>>;
                                        {
                                        container.emplace(
                                            std::piecewise_construct,
                                            std::forward_as_tuple(std::move(key)),
                                            std::forward_as_tuple())
                                        } -> std::same_as<std::pair<typename C::iterator, bool>>;
                                        {
                                        container.try_emplace(std::move(key))
                                        } -> std::same_as<std::pair<typename C::iterator, bool>>;
                                        {
                                        container.try_emplace(hint, std::move(key))
                                        } -> std::same_as<typename C::iterator>;
                                    };

                           // Mapped object copy
                           requires not std::copyable<typename C::mapped_type> or
                                    requires(typename C::mapped_type const& obj)
                                    {
                                        // Insertion
                                        {
                                        container.insert_or_assign(std::move(key), obj)
                                        } -> std::same_as<std::pair<typename C::iterator, bool>>;
                                        {
                                        container.insert_or_assign(hint, std::move(key), obj)
                                        } -> std::same_as<typename C::iterator>;

                                        // Emplacement (key move, mapped object copy constructor)
                                        {
                                        container.emplace(std::move(key), obj)
                                        } -> std::same_as<std::pair<typename C::iterator, bool>>;
                                        {
                                        container.emplace(
                                            std::piecewise_construct,
                                            std::forward_as_tuple(std::move(key)),
                                            std::forward_as_tuple(obj))
                                        } -> std::same_as<std::pair<typename C::iterator, bool>>;
                                        {
                                        container.try_emplace(std::move(key), obj)
                                        } -> std::same_as<std::pair<typename C::iterator, bool>>;
                                        {
                                        container.try_emplace(hint, std::move(key), obj)
                                        } -> std::same_as<typename C::iterator>;
                                    };

                           // Mapped object move
                           requires not std::movable<typename C::mapped_type> or
                                    requires(typename C::mapped_type&& obj)
                                    {
                                        // Insertion
                                        {
                                        container.insert_or_assign(std::move(key), std::move(obj))
                                        } -> std::same_as<std::pair<typename C::iterator, bool>>;
                                        {
                                        container.insert_or_assign(hint, std::move(key), std::move(obj))
                                        } -> std::same_as<typename C::iterator>;

                                        // Emplacement (key move, mapped object move constructor)
                                        {
                                        container.emplace(std::move(key), std::move(obj))
                                        } -> std::same_as<std::pair<typename C::iterator, bool>>;
                                        {
                                        container.emplace(
                                            std::piecewise_construct,
                                            std::forward_as_tuple(std::move(key)),
                                            std::forward_as_tuple(std::move(obj)))
                                        } -> std::same_as<std::pair<typename C::iterator, bool>>;
                                        {
                                        container.try_emplace(std::move(key), std::move(obj))
                                        } -> std::same_as<std::pair<typename C::iterator, bool>>;
                                        {
                                        container.try_emplace(hint, std::move(key), std::move(obj))
                                        } -> std::same_as<typename C::iterator>;
                                    };
                       };
          };
      };

    /// A map container with multiple key occurrence.
    /// Satisfied by multimap and unordered_multimap.
    template <typename C>
    concept multiple_map_container
    = map_container<C> and
      requires(C& container, C const& const_cont)
      {
          requires
          requires(typename C::const_iterator const& hint)
          {
              requires not std::copyable<typename C::key_type> or
                       requires(typename C::key_type const& key)
                       {
                           // Emplacement (key copy, mapped object default constructor)
                           requires not std::default_initializable<typename C::mapped_type> or
                                    requires
                                    {
                                        {
                                        container.emplace(key)
                                        } -> std::same_as<typename C::iterator>;
                                        {
                                        container.emplace(
                                            std::piecewise_construct,
                                            std::forward_as_tuple(key),
                                            std::forward_as_tuple())
                                        } -> std::same_as<typename C::iterator>;
                                    };

                           // Emplacement (key copy, mapped object copy constructor)
                           requires not std::copyable<typename C::mapped_type> or
                                    requires(typename C::mapped_type const& obj)
                                    {
                                        {
                                        container.emplace(key, obj)
                                        } -> std::same_as<typename C::iterator>;
                                        {
                                        container.emplace(
                                            std::piecewise_construct,
                                            std::forward_as_tuple(key),
                                            std::forward_as_tuple(obj))
                                        } -> std::same_as<typename C::iterator>;
                                    };

                           // Emplacement (key copy, mapped object move constructor)
                           requires not std::movable<typename C::mapped_type> or
                                    requires(typename C::mapped_type&& obj)
                                    {
                                        {
                                        container.emplace(key, std::move(obj))
                                        } -> std::same_as<typename C::iterator>;
                                        {
                                        container.emplace(
                                            std::piecewise_construct,
                                            std::forward_as_tuple(key),
                                            std::forward_as_tuple(std::move(obj)))
                                        } -> std::same_as<typename C::iterator>;
                                    };
                       };

              requires not std::movable<typename C::key_type> or
                       requires(typename C::key_type&& key)
                       {
                           // Emplacement (key move, mapped object default constructor)
                           requires not std::default_initializable<typename C::mapped_type> or
                                    requires
                                    {
                                        {
                                        container.emplace(std::move(key))
                                        } -> std::same_as<typename C::iterator>;
                                        {
                                        container.emplace(
                                            std::piecewise_construct,
                                            std::forward_as_tuple(std::move(key)),
                                            std::forward_as_tuple())
                                        } -> std::same_as<typename C::iterator>;
                                    };

                           // Emplacement (key copy, mapped object copy constructor)
                           requires not std::copyable<typename C::mapped_type> or
                                    requires(typename C::mapped_type const& obj)
                                    {
                                        {
                                        container.emplace(std::move(key), obj)
                                        } -> std::same_as<typename C::iterator>;
                                        {
                                        container.emplace(
                                            std::piecewise_construct,
                                            std::forward_as_tuple(std::move(key)),
                                            std::forward_as_tuple(obj))
                                        } -> std::same_as<typename C::iterator>;
                                    };

                           // Emplacement (key copy, mapped object move constructor)
                           requires not std::movable<typename C::mapped_type> or
                                    requires(typename C::mapped_type&& obj)
                                    {
                                        {
                                        container.emplace(std::move(key), std::move(obj))
                                        } -> std::same_as<typename C::iterator>;
                                        {
                                        container.emplace(
                                            std::piecewise_construct,
                                            std::forward_as_tuple(std::move(key)),
                                            std::forward_as_tuple(std::move(obj)))
                                        } -> std::same_as<typename C::iterator>;
                                    };
                       };
          };
      };

    /// An ordered map container.
    /// Satisfied by map and multimap.
    template <typename C>
    concept ordered_map_container = map_container<C> and ordered_associative_container<C>;

    /// An ordered map container without multiple key occurrence.
    /// Satisfied by map.
    template <typename C>
    concept ordered_unique_map_container
    = unique_map_container<C> and ordered_map_container<C>;

    /// An ordered map container with multiple key occurrence.
    /// Satisfied by multimap.
    template <typename C>
    concept ordered_multiple_map_container
    = multiple_map_container<C> and ordered_map_container<C>;

    /// A hash-based map container.
    /// Satisfied by unordered_map and unordered_multimap.
    template <typename C>
    concept unordered_map_container = map_container<C> and unordered_associative_container<C>;

    /// A hash-based map container without multiple key occurrence.
    /// Satisfied by unordered_map.
    template <typename C>
    concept unordered_unique_map_container
    = unique_map_container<C> and unordered_map_container<C>;

    /// A hash-based map container with multiple key occurrence.
    /// Satisfied by unordered_multimap.
    template <typename C>
    concept unordered_multiple_map_container
    = multiple_map_container<C> and unordered_map_container<C>;

    template <typename C, typename ValueType, typename KeyType = ValueType>
    concept associative_container_of
    = container_of<C, ValueType> and
      associative_container<C> and
      std::same_as<KeyType, typename C::key_type>;

    template <typename C, typename ValueType, typename KeyType = ValueType>
    concept unique_associative_container_of
    = associative_container_of<C, ValueType, KeyType> and unique_associative_container<C>;

    template <typename C, typename ValueType, typename KeyType = ValueType>
    concept multiple_associative_container_of
    = associative_container_of<C, ValueType, KeyType> and multiple_associative_container<C>;

    template <typename C, typename ValueType, typename KeyType = ValueType>
    concept ordered_associative_container_of
    = associative_container_of<C, ValueType, KeyType> and ordered_associative_container<C>;

    template <typename C, typename ValueType, typename KeyType = ValueType>
    concept ordered_unique_associative_container_of
    = unique_associative_container_of<C, ValueType, KeyType> and ordered_unique_associative_container<C>;

    template <typename C, typename ValueType, typename KeyType = ValueType>
    concept ordered_multiple_associative_container_of
    = multiple_associative_container_of<C, ValueType, KeyType> and ordered_multiple_associative_container<C>;

    template <typename C, typename ValueType, typename KeyType = ValueType>
    concept unordered_associative_container_of
    = associative_container_of<C, ValueType, KeyType> and unordered_associative_container<C>;

    template <typename C, typename ValueType, typename KeyType = ValueType>
    concept unordered_unique_associative_container_of
    = unique_associative_container_of<C, ValueType, KeyType> and unordered_unique_associative_container<C>;

    template <typename C, typename ValueType, typename KeyType = ValueType>
    concept unordered_multiple_associative_container_of
    = multiple_associative_container_of<C, ValueType, KeyType> and unordered_multiple_associative_container<C>;

    template <typename C, typename KeyType, typename MappedType>
    concept map_container_of
    = map_container<C> and
      std::same_as<KeyType, typename C::key_type> and
      std::same_as<MappedType, typename C::mapped_type>;

    template <typename C, typename KeyType, typename MappedType>
    concept unique_map_container_of
    = map_container_of<C, KeyType, MappedType> and unique_map_container<C>;

    template <typename C, typename KeyType, typename MappedType>
    concept multiple_map_container_of
    = map_container_of<C, KeyType, MappedType> and multiple_map_container<C>;

    template <typename C, typename KeyType, typename MappedType>
    concept ordered_map_container_of
    = map_container_of<C, KeyType, MappedType> and
      ordered_map_container<C>;

    template <typename C, typename KeyType, typename MappedType>
    concept ordered_unique_map_container_of
    = unique_map_container_of<C, KeyType, MappedType> and ordered_unique_map_container<C>;

    template <typename C, typename KeyType, typename MappedType>
    concept ordered_multiple_map_container_of
    = multiple_map_container_of<C, KeyType, MappedType> and ordered_multiple_map_container<C>;

    template <typename C, typename KeyType, typename MappedType>
    concept unordered_map_container_of
    = map_container_of<C, KeyType, MappedType> and
      unordered_map_container<C>;

    template <typename C, typename KeyType, typename MappedType>
    concept unordered_unique_map_container_of
    = unique_map_container_of<C, KeyType, MappedType> and unordered_unique_map_container<C>;

    template <typename C, typename KeyType, typename MappedType>
    concept unordered_multiple_map_container_of
    = multiple_map_container_of<C, KeyType, MappedType> and unordered_multiple_map_container<C>;

}