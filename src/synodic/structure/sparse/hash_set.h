#pragma once

#include <vector>

#include "synodic/exception/not_implemented.h"
#include "synodic/structure/sparse/vector.h"

namespace synodic::sparse
{
	template<class Key, class Hash = std::hash<Key>>
	class HashSet final
	{
		using size_type = size_t;

		static constexpr size_type blockSize_ = 8;

	public:
		HashSet()			  = default;
		~HashSet() override = default;

	private:
		SparseVector<Key, Hash> indirectionTable_;
		std::vector<Key> keys_;
	};

}