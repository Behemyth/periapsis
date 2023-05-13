#pragma once

#include <array>

#include "synodic/types/concepts.h"

namespace synodic::lockfree
{
	/**
	 * \brief An adapter for containers that enforce single producer, single consumer semantics on atomic data
	 * \tparam T The type to store
	 * \tparam Container The container
	 */
	template<typename T, sequence_container_of<T> Container>
	class SPSCQueue
	{
	public:
		using container_type  = Container;
		using value_type	  = typename Container::value_type;
		using size_type		  = typename Container::size_type;
		using reference		  = typename Container::reference;
		using const_reference = typename Container::const_reference;

		[[nodiscard]] bool empty() const noexcept;

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

	template<typename T, sequence_container_of<T> Container>
	bool SPSCQueue<T, Container>::empty() const noexcept
	{
		return writeIndex_.load(std::memory_order_acquire) == readIndex_.load(std::memory_order_acquire);
	}
}