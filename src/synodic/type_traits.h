#pragma once

#include <tuple>
#include <utility>

namespace synodic
{

	template<typename T, size_t N>
	struct Tag
	{
		// Returns the type associated with the tag
		friend auto GetType(Tag<T, N>);

		// Returns the ID of the tag
		consteval friend size_t GetID(Tag<T, N>);
	};

	// The definitions of friend functions. Prevent multiple instantiations by requiring the types
	//		to be different
	template<typename T, typename U, size_t N>
	requires(!std::is_same_v<T, U>) struct FriendType
	{
		friend auto GetType(Tag<T, N>)
		{
			return U {};
		}
		consteval friend size_t GetID(Tag<T, N>)
		{
			return 0;
		}
	};


	// This has a templated conversion operator which in turn triggers instantiations.
	template<typename T, size_t N>
	struct TypeConversion
	{
		template<typename U, size_t = sizeof(FriendType<T, U, N>)>
		consteval operator U()
		{
			return U();
		}
	};

	// The byproduct is instantiations.
	template<typename T, size_t... Ns>
	consteval auto ConstructorParameterCount(size_t) -> decltype(T(TypeConversion<T, Ns>()...), 0)
	{
		return sizeof...(Ns);
	}

	template<typename T, size_t... Ns>
	consteval size_t ConstructorParameterCount(...)
	{
		return ConstructorParameterCount<T, Ns..., sizeof...(Ns)>(0);
	}

	template<typename T, typename U>
	struct TagTuple;

	template<typename T, size_t... Ns>
	struct TagTuple<T, std::index_sequence<Ns...>>
	{
		using Type = std::tuple<decltype(GetType(Tag<T, Ns>()))...>;
	};


	// Exported types

	// Given a type T, returns a tuple of the parameters for the shortest constructor for that type
	template<typename T>
	using ConstructorParameters = typename TagTuple<
		T,
		std::make_index_sequence<ConstructorParameterCount<T>(0)>>::Type;

}