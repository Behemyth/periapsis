#pragma once

#include <concepts>
#include <type_traits>

namespace synodic
{
	template<class F, typename Ret, class... Args>
	concept callable = std::invocable<F, Args...> && std::same_as<Ret, std::invoke_result_t<F, Args...>>;

}