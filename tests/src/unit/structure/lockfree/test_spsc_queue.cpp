#include <gtest/gtest.h>

#include <array>
#include <deque>
#include <forward_list>
#include <list>
#include <string>
#include <vector>

#include "synodic/structure/lockfree/spsc_queue.h"
#include "synodic/types/concepts.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Types
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

using Array				= std::array<int, 3>;
using Deque				= std::deque<int>;
using ForwardList		= std::forward_list<int>;
using List				= std::list<int>;
using String			= std::basic_string<int>;
using Vector			= std::vector<int>;

template<typename T>
using Queue = synodic::lockfree::SPSCQueue<int, T>;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Tests
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

TEST(spsc_queue, standard_sequence_container)
{
	static_assert(synodic::sequence_container<Queue<Array>>);
	static_assert(synodic::sequence_container<Queue<Deque>>);
	static_assert(synodic::sequence_container<Queue<ForwardList>>);
	static_assert(synodic::sequence_container<Queue<List>>);
	static_assert(synodic::sequence_container<Queue<String>>);
	static_assert(synodic::sequence_container<Queue<Vector>>);
}