export module synodic.library.function:function_ref;

import synodic.library.type.concepts;

// Temporary implementation while the basis is considered for standardization.
//	As a result, this implementation follows stl naming conventions
export namespace synodic
{
	template<class F>
	class FunctionRef;

	template<class R, class... Args>
	class FunctionRef<R(Args...)>
	{
	public:
		constexpr FunctionRef() noexcept = delete;

		// Construct the FunctionRef referring to the same Callable as the input
		constexpr FunctionRef(const FunctionRef<R(Args...)>& function) noexcept = default;

		// Constructs from a Callable object
		template<callable<R, Args...> Fn>
		constexpr FunctionRef(Fn&& function) noexcept;

		constexpr ~FunctionRef() = default;

		// Assigns this FunctionRef the same Callable as the input
		constexpr FunctionRef<R(Args...)>& operator=(const FunctionRef<R(Args...)>& other) noexcept = default;

		template<callable<R, Args...> Fn>
		constexpr FunctionRef<R(Args...)>& operator=(Fn&& function) noexcept;

		constexpr R operator()(Args... arguments) const;
		constexpr void swap(FunctionRef& other) noexcept;

	private:
		void* object_				   = nullptr;
		R (*callback_)(void*, Args...) = nullptr;
	};
}

namespace synodic
{

	template<class R, class... Args>
	template<callable<R, Args...> Fn>
	constexpr FunctionRef<R(Args...)>::FunctionRef(Fn&& function) noexcept :
		object_(const_cast<void*>(reinterpret_cast<const void*>(std::addressof(function)))),
		callback_(
			[](void* object, Args... arguments) -> R
			{
				return std::invoke(*reinterpret_cast<std::add_pointer_t<Fn>>(object), std::forward<Args>(arguments)...);
			})
	{
	}

	template<class R, class... Args>
	template<callable<R, Args...> Fn>
	constexpr FunctionRef<R(Args...)>& FunctionRef<R(Args...)>::operator=(Fn&& function) noexcept
	{
		object_	  = reinterpret_cast<void*>(std::addressof(function));
		callback_ = [](void* object, Args... arguments)
		{
			return std::invoke(*reinterpret_cast<std::add_pointer_t<Fn>>(object), std::forward<Args>(arguments)...);
		};

		return *this;
	}

	template<class R, class... Args>
	constexpr R FunctionRef<R(Args...)>::operator()(Args... arguments) const
	{
		return std::invoke(callback_, object_, std::forward<Args>(arguments)...);
	}

	template<class R, class... Args>
	constexpr void FunctionRef<R(Args...)>::swap(FunctionRef& other) noexcept
	{
		std::swap(object_, other.object_);
		std::swap(callback_, other.callback_);
	}

	template<class Fn>
	constexpr void swap(FunctionRef<Fn>& left, FunctionRef<Fn>& right) noexcept
	{
		left.swap(right);
	}

	// Function pointer deduction guide
	template<typename R, typename... Args>
	FunctionRef(R (*)(Args...)) -> FunctionRef<R(Args...)>;

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
	struct SignatureStripper<R (T::*)(Args...) & noexcept>
	{
		using type = R(Args...);
	};
	template<class R, class T, class... Args>
	struct SignatureStripper<R (T::*)(Args...) const & noexcept>
	{
		using type = R(Args...);
	};

	export template<class Fn, class Stripped = typename SignatureStripper<decltype(&Fn::operator())>::type>
	FunctionRef(Fn) -> FunctionRef<Stripped>;

}
