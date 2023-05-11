#pragma once

#include <array>

namespace synodic
{
	/**
	 * \brief A container adapter that closely follows the interface of std::queue while implemented as a ring buffer
	 * \tparam T The type to store
	 * \tparam Container The container
	 */
	template<typename T, typename Container>
	class Queue
	{
	public:
		using container_type  = Container;
		using value_type	  = typename Container::value_type;
		using size_type		  = typename Container::size_type;
		using reference		  = typename Container::reference;
		using const_reference = typename Container::const_reference;

		Queue();
		~Queue() = default;

		Queue(const Queue&)	  = delete;
		Queue(Queue&&) noexcept = default;

		Queue& operator=(const Queue&)	 = delete;
		Queue& operator=(Queue&&) noexcept = default;

		bool operator==(const Queue& other);
		bool operator==(Queue& other);

		[[nodiscard]] const T& Back() const;

		const T& Front();

		Reference operator[](std::size_t i);

		void Push(const T&);
		void Push(T&&);

		[[nodiscard]] SizeType Size() const;

	private:
		SizeType front_;
		SizeType size_;

		std::array<T, Capacity> data_;

		void Push_();
	};
}