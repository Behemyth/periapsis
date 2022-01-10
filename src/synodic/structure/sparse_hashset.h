#pragma once

#include <vector>

#include "SparseStructure.h"
#include "SparseVector.h"

namespace synodic
{
	template<class Key, class Hash = std::hash<Key>>
	class SparseHashSet final : public SparseStructure
	{
		using size_type = size_t;

		static constexpr size_type blockSize_ = 8;

	public:
		SparseHashSet()			  = default;
		~SparseHashSet() override = default;

	private:
		SparseVector<Key, Hash> indirectionTable_;
		std::vector<Key> keys_;
	};

}