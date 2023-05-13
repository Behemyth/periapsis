#include <gtest/gtest.h>

#include <array>
#include <deque>
#include <forward_list>
#include <list>
#include <map>
#include <set>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "synodic/structure/lockfree/spsc_queue.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Types
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

using Array				= std::array<int, 3>;
using Deque				= std::deque<int>;
using ForwardList		= std::forward_list<int>;
using List				= std::list<int>;
using Map				= std::map<int, int>;
using MultiMap			= std::multimap<int, int>;
using Set				= std::set<int>;
using MultiSet			= std::multiset<int>;
using String			= std::basic_string<int>;
using UnorderedMap		= std::unordered_map<int, int>;
using UnorderedMultiMap = std::unordered_multimap<int, int>;
using UnorderedSet		= std::unordered_set<int>;
using UnorderedMultiSet = std::unordered_multiset<int>;
using Vector			= std::vector<int>;

template<typename T>
using Queue = synodic::lockfree::SPSCQueue<int, T>;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Tests
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * \brief Verifies that the expected supported standard types fulfill the queue concept
 */
TEST(spsc_queue, standard_sequence_container)
{
	static_assert(synodic::lockfree::spsc_queue_container<Queue<Array>>);
	static_assert(synodic::lockfree::spsc_queue_container<Queue<Deque>>);
	static_assert(synodic::lockfree::spsc_queue_container<Queue<List>>);
	static_assert(synodic::lockfree::spsc_queue_container<Queue<String>>);
	static_assert(synodic::lockfree::spsc_queue_container<Queue<Vector>>);

	static_assert(not synodic::lockfree::spsc_queue_container<Queue<ForwardList>>);
	static_assert(not synodic::lockfree::spsc_queue_container<Queue<Map>>);
	static_assert(not synodic::lockfree::spsc_queue_container<Queue<MultiMap>>);
	static_assert(not synodic::lockfree::spsc_queue_container<Queue<Set>>);
	static_assert(not synodic::lockfree::spsc_queue_container<Queue<MultiSet>>);
	static_assert(not synodic::lockfree::spsc_queue_container<Queue<UnorderedMap>>);
	static_assert(not synodic::lockfree::spsc_queue_container<Queue<UnorderedMultiMap>>);
	static_assert(not synodic::lockfree::spsc_queue_container<Queue<UnorderedSet>>);
	static_assert(not synodic::lockfree::spsc_queue_container<Queue<UnorderedMultiSet>>);
}