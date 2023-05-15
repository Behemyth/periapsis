// Concepts used by other modules and exported

export module synodic.library.type.concepts:shared;

import <concepts>;

export namespace synodic
{
	// Verifies that the given type is the same as its decayed type
	template<typename T>
	concept decayed = std::same_as<T, std::decay_t<T>>;
}
