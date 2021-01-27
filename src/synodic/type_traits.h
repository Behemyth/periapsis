#pragma once

#include <tuple>
#include <utility>

namespace synodic
{

	template<typename T, int N>
	struct Tag
	{
		// Returns the type associated with the tag
		friend auto GetType(Tag<T, N>);

		// Returns the ID of the tag
		consteval friend int GetID(Tag<T, N>);
	};

	// The definitions of friend functions. Prevent multiple instantiations by requiring the types
	//		to be different
	template<typename T, typename U, int N>
	requires !std::is_same_v<T, U> struct FriendType
	{
		friend auto GetType(Tag<T, N>)
		{
			return U{};
		}
		consteval friend int GetID(Tag<T, N>)
		{
			return N;
		}
	};


	// This has a templated conversion operator which in turn triggers instantiations.
	template<typename T, int N>
	struct TypeConversion
	{
		template<typename U, int = sizeof(FriendType<T, U, N>)>
		operator U();
	};

	// The byproduct is instantiations.
	template<typename T, int... Ns>
	consteval auto ConstructorParameterCount() -> decltype(T(TypeConversion<T, Ns> {}...), 0)
	{
		return sizeof...(Ns);
	}

	template<typename T, int... Ns>
	consteval int ConstructorParameterCount(...)
	{
		return ConstructorParameterCount<T, Ns..., sizeof...(Ns)>();
	}

	template<typename T, typename U>
	struct LoopholeTuple;

	template<typename T, int... Ns>
	struct LoopholeTuple<T, std::integer_sequence<int, Ns...>>
	{
		using Type = std::tuple<decltype(GetType(Tag<T, Ns> {}))...>;
	};


	// Exported types

	template<typename T>
	using ConstructorParameters =
		typename LoopholeTuple<T, std::make_integer_sequence<int, ConstructorParameterCount<T>()>>::
			Type;

}