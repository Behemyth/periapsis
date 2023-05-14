#pragma once

#include <array>
#include <optional>

#include "synodic/exception/not_implemented.h"
#include "synodic/types/concepts.h"

// TODO: Write note that internal backing container size/capacity tracking is wasted

namespace synodic::lockfree
{
	// Defines the set of containers that SPSCQueue supports
	template<typename T>
	concept spsc_queue_container = sequence_container<T> and double_ended_container<T> && sized_container<T>;

	// Convenience concept for identifying SPSCQueue backing containers
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
		using container_type		 = Container;
		using value_type			 = typename Container::value_type;
		using size_type				 = typename Container::size_type;
		using difference_type		 = typename Container::difference_type;
		using reference				 = typename Container::reference;
		using const_reference		 = typename Container::const_reference;
		using iterator				 = typename Container::iterator;
		using const_iterator		 = typename Container::const_iterator;
		using reverse_iterator		 = typename Container::reverse_iterator;
		using const_reverse_iterator = typename Container::const_reverse_iterator;

		// Construction

		constexpr SPSCQueue();
		constexpr explicit SPSCQueue(const Container& container);
		constexpr explicit SPSCQueue(Container&& container);
		constexpr SPSCQueue(const SPSCQueue& other) = delete;
		constexpr SPSCQueue(SPSCQueue&& other) noexcept = delete;

		template<class InputIt>
		constexpr SPSCQueue(InputIt first, InputIt last);

		template<class Alloc>
		constexpr explicit SPSCQueue(const Alloc& alloc) noexcept;

		template<class Alloc>
		constexpr SPSCQueue(const Container& container, const Alloc& alloc);

		template<class Alloc>
		constexpr SPSCQueue(Container&& container, const Alloc& alloc);

		template<class InputIt, class Alloc>
		constexpr SPSCQueue(InputIt first, InputIt last, const Alloc& alloc);

		// Destructor

		constexpr ~SPSCQueue() = default;

		// Assignment

		constexpr SPSCQueue& operator=(const SPSCQueue& other) = delete;
		constexpr SPSCQueue& operator=(SPSCQueue&& other) noexcept = delete;

		// Accessors

		constexpr reference front();
		constexpr const_reference front() const;
		constexpr reference back();
		constexpr const_reference back() const;

		/**
		 * \brief Moves the wrapped container from the queue
		 * \return The underlying container
		 */
		constexpr container_type extract() noexcept;

		// Capacity

		[[nodiscard]] constexpr bool empty() const noexcept;
		constexpr size_type size() const noexcept;
		constexpr size_type max_size() const noexcept;

		// Modifiers

		constexpr void push(const T& value);
		constexpr void push(T&& value);

		constexpr bool try_push(const T& value);
		constexpr bool try_push(T&& value);

		constexpr void pop();

		template<class... Args>
		constexpr void emplace(Args&&... args);

		template<class... Args>
		constexpr bool try_emplace(Args&&... args);

		// Comparison

		constexpr auto operator<=>(const SPSCQueue&) const = default;

	private:
		static constexpr size_t cacheLineSize = std::hardware_destructive_interference_size;

		alignas(cacheLineSize) Container container_;

		// Track capacity external to the container to guarantee alignment
		alignas(cacheLineSize) size_type capacity_;

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
		throw NotImplemented();
	}

	template<typename T, spsc_queue_container_of<T> Container>
	constexpr typename SPSCQueue<T, Container>::const_reference SPSCQueue<T, Container>::front() const
	{
		throw NotImplemented();
	}

	template<typename T, spsc_queue_container_of<T> Container>
	constexpr typename SPSCQueue<T, Container>::reference SPSCQueue<T, Container>::back()
	{
		throw NotImplemented();
	}

	template<typename T, spsc_queue_container_of<T> Container>
	constexpr typename SPSCQueue<T, Container>::const_reference SPSCQueue<T, Container>::back() const
	{
		throw NotImplemented();
	}

	template<typename T, spsc_queue_container_of<T> Container>
	constexpr typename SPSCQueue<T, Container>::container_type SPSCQueue<T, Container>::extract() noexcept
	{
		capacity_ = 0;

		return std::move(container_);
	}

	template<typename T, spsc_queue_container_of<T> Container>
	constexpr SPSCQueue<T, Container>::SPSCQueue() :
		SPSCQueue(Container())
	{
		throw NotImplemented();
	}

	template<typename T, spsc_queue_container_of<T> Container>
	constexpr SPSCQueue<T, Container>::SPSCQueue(const Container& container)
	{
		throw NotImplemented();
	}

	template<typename T, spsc_queue_container_of<T> Container>
	constexpr SPSCQueue<T, Container>::SPSCQueue(Container&& container)
	{
		throw NotImplemented();
	}

	template<typename T, spsc_queue_container_of<T> Container>
	template<class InputIt>
	constexpr SPSCQueue<T, Container>::SPSCQueue(InputIt first, InputIt last)
	{
		throw NotImplemented();
	}

	template<typename T, spsc_queue_container_of<T> Container>
	template<class Alloc>
	constexpr SPSCQueue<T, Container>::SPSCQueue(const Alloc& alloc) noexcept
	{
		throw NotImplemented();
	}

	template<typename T, spsc_queue_container_of<T> Container>
	template<class Alloc>
	constexpr SPSCQueue<T, Container>::SPSCQueue(const Container& container, const Alloc& alloc)
	{
		throw NotImplemented();
	}

	template<typename T, spsc_queue_container_of<T> Container>
	template<class Alloc>
	constexpr SPSCQueue<T, Container>::SPSCQueue(Container&& container, const Alloc& alloc)
	{
		throw NotImplemented();
	}

	template<typename T, spsc_queue_container_of<T> Container>
	template<class InputIt, class Alloc>
	constexpr SPSCQueue<T, Container>::SPSCQueue(InputIt first, InputIt last, const Alloc& alloc)
	{
		throw NotImplemented();
	}

	template<typename T, spsc_queue_container_of<T> Container>
	constexpr bool SPSCQueue<T, Container>::empty() const noexcept
	{
		return writeIndex_.load(std::memory_order_acquire) == readIndex_.load(std::memory_order_acquire);
	}

	template<typename T, spsc_queue_container_of<T> Container>
	constexpr typename SPSCQueue<T, Container>::size_type SPSCQueue<T, Container>::size() const noexcept
	{
		std::ptrdiff_t diff = writeIndex_.load(std::memory_order_acquire) - readIndex_.load(std::memory_order_acquire);
		if(diff < 0)
		{
			diff += capacity_;
		}
		return static_cast<size_type>(diff);
	}

	template<typename T, spsc_queue_container_of<T> Container>
	constexpr typename SPSCQueue<T, Container>::size_type SPSCQueue<T, Container>::max_size() const noexcept
	{
		return container_.max_size();
	}

	template<typename T, spsc_queue_container_of<T> Container>
	constexpr void SPSCQueue<T, Container>::push(const T& value)
	{
		throw NotImplemented();
	}

	template<typename T, spsc_queue_container_of<T> Container>
	constexpr void SPSCQueue<T, Container>::push(T&& value)
	{
		throw NotImplemented();
	}

	template<typename T, spsc_queue_container_of<T> Container>
	constexpr bool SPSCQueue<T, Container>::try_push(const T& value)
	{
		throw NotImplemented();
	}

	template<typename T, spsc_queue_container_of<T> Container>
	constexpr bool SPSCQueue<T, Container>::try_push(T&& value)
	{
		throw NotImplemented();
	}

	template<typename T, spsc_queue_container_of<T> Container>
	constexpr void SPSCQueue<T, Container>::pop()
	{
		throw NotImplemented();
	}

	template<typename T, spsc_queue_container_of<T> Container>
	template<class... Args>
	constexpr void SPSCQueue<T, Container>::emplace(Args&&... args)
	{
		throw NotImplemented();
	}

	template<typename T, spsc_queue_container_of<T> Container>
	template<class... Args>
	constexpr bool SPSCQueue<T, Container>::try_emplace(Args&&... args)
	{
		throw NotImplemented();
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Deduction guides as specified: https://en.cppreference.com/w/cpp/container/queue/deduction_guides
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	template<class Container>
	SPSCQueue(Container) -> SPSCQueue<typename Container::value_type, Container>;

	template<class Container, class Alloc>
	SPSCQueue(Container, Alloc) -> SPSCQueue<typename Container::value_type, Container>;

}