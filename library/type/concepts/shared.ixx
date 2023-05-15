// Shared concepts used by other partitions

export module synodic.library.type.concepts:shared;

import <concepts>;

export namespace synodic
{
	// Verifies that the given type is the same as its decayed type
	template<typename T>
	concept decayed = std::same_as<T, std::decay_t<T>>;

	// Implements the missing concept for 'is_invocable_r'
	template<typename Fn, typename R, typename... Args>
	concept invocable_r = std::is_invocable_r_v<R, Fn, Args...>;

	// An alias for 'invocable_r'
	template<typename Fn, typename R, typename... Args>
	concept callable = invocable_r<Fn, R, Args...>;

	// A concept implementation of the named requirement 'Hash' with Standard naming styles
	//		This definition is devoid of backwards compatibility requirements
	//		https://en.cppreference.com/w/cpp/named_req/Hash
	template<typename Fn, typename KeyType>
	concept hash_function = callable<const Fn, auto(KeyType&)->std::size_t, auto(const KeyType&)->std::size_t>;
}
