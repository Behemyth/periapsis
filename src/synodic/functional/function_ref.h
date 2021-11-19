#pragma once

#include <utility>

#include "synodic/type/concepts.h"

// Temporary implementation while the basis is considered for standardization.
//	As a result, this implementation follows stl naming conventions
namespace synodic
{
	template<class F>
	class function_ref;

	template<class R, class... Args>
	class function_ref<R(Args...)>
	{
	public:
		constexpr function_ref() noexcept = delete;

		// Construct the function_ref referring to the same Callable as the input
		constexpr function_ref(const function_ref<R(Args...)>& function) noexcept = default;

		// Constructs from a Callable object
		template<callable<R, Args...> Fn>
		constexpr function_ref(Fn&& function) noexcept;

		constexpr ~function_ref() = default;

		// Assigns this function_ref the same Callable as the input
		constexpr function_ref<R(Args...)>& operator=(const function_ref<R(Args...)>& other) noexcept = default;

		template<callable<R, Args...> Fn>
		constexpr function_ref<R(Args...)>& operator=(Fn&& function) noexcept;

		constexpr R operator()(Args... arguments) const;
		constexpr void swap(function_ref& other) noexcept;

	private:
		void* object_				   = nullptr;
		R (*callback_)(void*, Args...) = nullptr;
	};

	template<class R, class... Args>
	template<callable<R, Args...> Fn>
	constexpr function_ref<R(Args...)>::function_ref(Fn&& function) noexcept :
		object_(const_cast<void*>(reinterpret_cast<const void*>(std::addressof(function))))
	{
		callback_ = [](void* object, Args... arguments) -> R
		{
			return std::invoke(
				*reinterpret_cast<typename std::add_pointer<Fn>::type>(object),
				std::forward<Args>(arguments)...);
		};
	}

	template<class R, class... Args>
	template<callable<R, Args...> Fn>
	constexpr function_ref<R(Args...)>& function_ref<R(Args...)>::operator=(Fn&& function) noexcept
	{
		object_	  = reinterpret_cast<void*>(std::addressof(function));
		callback_ = [](void* object, Args... arguments)
		{
			return std::invoke(
				*reinterpret_cast<typename std::add_pointer<Fn>::type>(object),
				std::forward<Args>(arguments)...);
		};

		return *this;
	}

	template<class R, class... Args>
	constexpr R function_ref<R(Args...)>::operator()(Args... arguments) const
	{
		return std::invoke(callback_, object_, std::forward<Args>(arguments)...);
	}

	template<class R, class... Args>
	constexpr void function_ref<R(Args...)>::swap(function_ref& other) noexcept
	{
		std::swap(object_, other.object_);
		std::swap(callback_, other.callback_);
	}

	template<class Fn>
	constexpr void swap(function_ref<Fn>& left, function_ref<Fn>& right) noexcept
	{
		left.swap(right);
	}

	// Function pointer deduction guide
	template<typename R, typename... Args>
	function_ref(R (*)(Args...)) -> function_ref<R(Args...)>;

	// Signature deduction guide
	// STD: Only participates in overload resolution if &F::operator() is well-formed
	//		when treated as an unevaluated operand and decltype(&F::operator()) is of the form
	//		R(G::*)(A...) (optionally cv-qualified, optionally noexcept, optionally lvalue reference
	//		qualified) for some class type G. The deduced type is std::function<R(A...)>.
	template<class Fn>
	struct SignatureStripper;

	template<class R, class T, class... Args>
	struct SignatureStripper<R (T::*)(Args...)>
	{
		using type = R(Args...);
	};
	template<class R, class T, class... Args>
	struct SignatureStripper<R (T::*)(Args...) const>
	{
		using type = R(Args...);
	};
	template<class R, class T, class... Args>
	struct SignatureStripper<R (T::*)(Args...)&>
	{
		using type = R(Args...);
	};
	template<class R, class T, class... Args>
	struct SignatureStripper<R (T::*)(Args...) const&>
	{
		using type = R(Args...);
	};
	template<class R, class T, class... Args>
	struct SignatureStripper<R (T::*)(Args...) noexcept>
	{
		using type = R(Args...);
	};
	template<class R, class T, class... Args>
	struct SignatureStripper<R (T::*)(Args...) const noexcept>
	{
		using type = R(Args...);
	};
	template<class R, class T, class... Args>
	struct SignatureStripper<R (T::*)(Args...)& noexcept>
	{
		using type = R(Args...);
	};
	template<class R, class T, class... Args>
	struct SignatureStripper<R (T::*)(Args...) const& noexcept>
	{
		using type = R(Args...);
	};

	template<class Fn, class Stripped = typename SignatureStripper<decltype(&Fn::operator())>::type>
	function_ref(Fn) -> function_ref<Stripped>;

}