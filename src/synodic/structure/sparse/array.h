#pragma once

#include <array>

#include "synodic/exception/not_implemented.h"
#include "synodic/structure/sparse/bitmap.h"

namespace synodic::sparse
{
	template<class T, size_t N, size_t GroupSize>
	class Array final
	{
		using size_type = size_t;

		static constexpr size_type blockCount_ = (N + GroupSize - 1) / GroupSize;

	public:
		Array()			= default;
		~Array() = default;

		T& operator[](size_type);
		bool Test(size_type) const;

	private:
		std::array<BitMap<T, GroupSize>, blockCount_> data_;
	};

	template<class T, size_t N, size_t GroupSize>
	T& Array<T, N, GroupSize>::operator[](size_type index)
	{
		throw NotImplemented();
	}

	template<class T, size_t N, size_t GroupSize>
	bool Array<T, N, GroupSize>::Test(size_type index) const
	{
		throw NotImplemented();
	}

}