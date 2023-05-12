#pragma once

#include <vector>

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
		~HashSet() = default;

	private:
		Vector<Key, 4> indirectionTable_;
		std::vector<Key> keys_;
	};

}