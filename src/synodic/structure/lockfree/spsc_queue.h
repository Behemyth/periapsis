#pragma once

#include <array>

#include "synodic/types/concepts.h"

namespace synodic::lockfree
{

	// Defines a subset of the named requirement SequenceContainer that SPSCQueue supports
	template<typename T>
	concept spsc_queue_container = sequence_container<T> && inplace_back_constructing_container<T>;

	// Convenience concept for identifying containers for SPSCQueue
	template<typename T, typename ValueType>
	concept spsc_queue_container_of = container_of<T, ValueType> && spsc_queue_container<T>;

	/**
	 * \brief An adapter for containers that enforce single producer, single consumer atomic access
	 * \tparam T The type to store
	 * \tparam Container The container for storage backing
	 */
	template<typename T, spsc_queue_container_of<T> Container>
	class SPSCQueue
	{
	public:
		using container_type  = Container;
		using value_type	  = typename Container::value_type;
		using size_type		  = typename Container::size_type;
		using difference_type = typename Container::difference_type;
		using reference		  = typename Container::reference;
		using const_reference = typename Container::const_reference;
		using iterator		  = typename Container::iterator;
		using const_iterator  = typename Container::const_iterator;

		// Construction
		SPSCQueue();
		explicit SPSCQueue(const Container& container);
		explicit SPSCQueue(Container&& container);
		SPSCQueue(const SPSCQueue& other);
		SPSCQueue(SPSCQueue&& other);

		template<class InputIt>
		SPSCQueue(InputIt first, InputIt last);

		template<class Alloc>
		explicit SPSCQueue(const Alloc& alloc);

		template<class Alloc>
		SPSCQueue(const Container& container, const Alloc& alloc);

		template<class Alloc>
		SPSCQueue(Container&& container, const Alloc& alloc);

		template<class Alloc>
		SPSCQueue(const SPSCQueue& other, const Alloc& alloc);

		template<class Alloc>
		SPSCQueue(SPSCQueue&& other, const Alloc& alloc);

		template<class InputIt, class Alloc>
		SPSCQueue(InputIt first, InputIt last, const Alloc& alloc);

		// Assignment
		SPSCQueue& operator=(const SPSCQueue& other);
		SPSCQueue& operator=(SPSCQueue&& other);

		// Accessors
		constexpr reference front();
		constexpr const_reference front() const;

		/**
		 * \brief Not thread-safe
		 * \return Iterator
		 */
		constexpr iterator begin() noexcept;

		/**
		 * \brief Not thread-safe
		 * \return Constant iterator
		 */
		constexpr const_iterator begin() const noexcept;

		/**
		 * \brief Not thread-safe
		 * \return Iterator
		 */
		constexpr iterator end() noexcept;

		/**
		 * \brief Not thread-safe
		 * \return Constant iterator
		 */
		constexpr const_iterator end() const noexcept;

		/**
		 * \brief Not thread-safe
		 * \return Constant iterator
		 */
		constexpr const_iterator cbegin() const noexcept;

		/**
		 * \brief Not thread-safe
		 * \return Constant iterator
		 */
		constexpr const_iterator cend() const noexcept;

		// Capacity
		[[nodiscard]] constexpr bool empty() const noexcept;
		constexpr size_type max_size() const noexcept;

		// Modifiers

		auto operator<=>(const SPSCQueue&) const = default;

	private:
		// Thank you standard writers for including this system information
		static constexpr size_t cacheLineSize = std::hardware_destructive_interference_size;

		alignas(cacheLineSize) Container container_;

		alignas(cacheLineSize) std::atomic<size_type> writeIndex_;
		alignas(cacheLineSize) size_type cachedReadIndex_;
		alignas(cacheLineSize) std::atomic<size_type> readIndex_;
		alignas(cacheLineSize) size_type cachedWriteIndex_;

		// The padding used to avoid cache line overlap with the cached write index
		std::array<std::byte, cacheLineSize - sizeof cachedWriteIndex_> padding_;
	};

	template<typename T, spsc_queue_container_of<T> Container>
	constexpr typename SPSCQueue<T, Container>::reference SPSCQueue<T, Container>::front()
	{
	}

	template<typename T, spsc_queue_container_of<T> Container>
	constexpr typename SPSCQueue<T, Container>::const_reference SPSCQueue<T, Container>::front() const
	{
	}

	template<typename T, spsc_queue_container_of<T> Container>
	constexpr typename SPSCQueue<T, Container>::iterator SPSCQueue<T, Container>::begin() noexcept
	{
	}

	template<typename T, spsc_queue_container_of<T> Container>
	constexpr typename SPSCQueue<T, Container>::const_iterator SPSCQueue<T, Container>::begin() const noexcept
	{
	}

	template<typename T, spsc_queue_container_of<T> Container>
	constexpr typename SPSCQueue<T, Container>::iterator SPSCQueue<T, Container>::end() noexcept
	{
	}

	template<typename T, spsc_queue_container_of<T> Container>
	constexpr typename SPSCQueue<T, Container>::const_iterator SPSCQueue<T, Container>::end() const noexcept
	{
	}

	template<typename T, spsc_queue_container_of<T> Container>
	constexpr typename SPSCQueue<T, Container>::const_iterator SPSCQueue<T, Container>::cbegin() const noexcept
	{
	}

	template<typename T, spsc_queue_container_of<T> Container>
	constexpr typename SPSCQueue<T, Container>::const_iterator SPSCQueue<T, Container>::cend() const noexcept
	{
	}

	template<typename T, spsc_queue_container_of<T> Container>
	SPSCQueue<T, Container>::SPSCQueue() :
		SPSCQueue(Container())
	{
	}

	template<typename T, spsc_queue_container_of<T> Container>
	SPSCQueue<T, Container>::SPSCQueue(const Container& container)
	{
	}

	template<typename T, spsc_queue_container_of<T> Container>
	SPSCQueue<T, Container>::SPSCQueue(Container&& container)
	{
	}

	template<typename T, spsc_queue_container_of<T> Container>
	SPSCQueue<T, Container>::SPSCQueue(const SPSCQueue& other)
	{
	}

	template<typename T, spsc_queue_container_of<T> Container>
	SPSCQueue<T, Container>::SPSCQueue(SPSCQueue&& other)
	{
	}

	template<typename T, spsc_queue_container_of<T> Container>
	template<class InputIt>
	SPSCQueue<T, Container>::SPSCQueue(InputIt first, InputIt last)
	{
	}

	template<typename T, spsc_queue_container_of<T> Container>
	template<class Alloc>
	SPSCQueue<T, Container>::SPSCQueue(const Alloc& alloc)
	{
	}

	template<typename T, spsc_queue_container_of<T> Container>
	template<class Alloc>
	SPSCQueue<T, Container>::SPSCQueue(const Container& container, const Alloc& alloc)
	{
	}

	template<typename T, spsc_queue_container_of<T> Container>
	template<class Alloc>
	SPSCQueue<T, Container>::SPSCQueue(Container&& container, const Alloc& alloc)
	{
	}

	template<typename T, spsc_queue_container_of<T> Container>
	template<class Alloc>
	SPSCQueue<T, Container>::SPSCQueue(const SPSCQueue& other, const Alloc& alloc)
	{
	}

	template<typename T, spsc_queue_container_of<T> Container>
	template<class Alloc>
	SPSCQueue<T, Container>::SPSCQueue(SPSCQueue&& other, const Alloc& alloc)
	{
	}

	template<typename T, spsc_queue_container_of<T> Container>
	template<class InputIt, class Alloc>
	SPSCQueue<T, Container>::SPSCQueue(InputIt first, InputIt last, const Alloc& alloc)
	{
	}

	template<typename T, spsc_queue_container_of<T> Container>
	SPSCQueue<T, Container>& SPSCQueue<T, Container>::operator=(const SPSCQueue& other)
	{
	}

	template<typename T, spsc_queue_container_of<T> Container>
	SPSCQueue<T, Container>& SPSCQueue<T, Container>::operator=(SPSCQueue&& other)
	{
	}

	template<typename T, spsc_queue_container_of<T> Container>
	constexpr bool SPSCQueue<T, Container>::empty() const noexcept
	{
		return writeIndex_.load(std::memory_order_acquire) == readIndex_.load(std::memory_order_acquire);
	}

	template<typename T, spsc_queue_container_of<T> Container>
	constexpr typename SPSCQueue<T, Container>::size_type SPSCQueue<T, Container>::max_size() const noexcept
	{
		return container_.max_size();
	}
}