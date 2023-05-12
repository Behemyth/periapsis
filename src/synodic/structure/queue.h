#pragma once

#include "synodic/types/concepts.h"

namespace synodic
{
	/**
	 * \brief A container adapter that closely follows the interface of std::queue with a ring buffer backend
	 * \tparam T The type to store
	 * \tparam Container The container
	 */
	template<typename T, container Container>
	class Queue
	{
	public:
		using container_type  = Container;
		using value_type	  = typename Container::value_type;
		using size_type		  = typename Container::size_type;
		using reference		  = typename Container::reference;
		using const_reference = typename Container::const_reference;

		Queue();
		explicit Queue(const Container& container);
		explicit Queue(Container&& container);

		~Queue() = default;

		[[nodiscard]] bool Empty() const;

	private:
		size_type front_;
		size_type size_;

		Container data_;
	};

	template<typename T, container Container>
	Queue<T, Container>::Queue() :
		Queue(Container())
	{
	}

	template<typename T, container Container>
	Queue<T, Container>::Queue(const Container& container)
	{
	}

	template<typename T, container Container>
	Queue<T, Container>::Queue(Container&& container)
	{
	}

	template <typename T, container Container>
	bool Queue<T, Container>::Empty() const
	{
		return data_.empty();
	}

	// Deduction guides as specified: https://en.cppreference.com/w/cpp/container/queue/deduction_guides

	template<class Container>
	Queue(Container) -> Queue<typename Container::value_type, Container>;

	template<class Container, class Alloc>
	Queue(Container, Alloc) -> Queue<typename Container::value_type, Container>;
}