/*
A subset of std::generator (P2502R2) for standard libraries that do not provide <generator> yet.

When the standard library defines std::generator, this module exports nothing and the standard
type is used.

Not provided:
- Yielding a nested generator through std::ranges::elements_of.
- Allocator support. The Allocator parameter is accepted for source compatibility and ignored.
 */

module;
#include <version>

export module generator;

import std;

#ifndef __cpp_lib_generator

namespace std
{

	export template<class Ref, class V = void, class Allocator = void>
	class generator : public ranges::view_interface<generator<Ref, V, Allocator>>
	{
		using value = conditional_t<is_void_v<V>, remove_cvref_t<Ref>, V>;
		using reference = conditional_t<is_void_v<V>, Ref&&, Ref>;

		class iterator;

	public:
		using yielded = conditional_t<is_reference_v<reference>, reference, const reference&>;

		class promise_type
		{
		public:
			generator get_return_object() noexcept
			{
				return generator(coroutine_handle<promise_type>::from_promise(*this));
			}

			suspend_always initial_suspend() const noexcept
			{
				return {};
			}

			suspend_always final_suspend() const noexcept
			{
				return {};
			}

			// The yielded object outlives the suspension, so the iterator can refer to it
			suspend_always yield_value(yielded value) noexcept
			{
				value_ = addressof(value);
				return {};
			}

			// A generator of rvalue references yields a copy of an lvalue
			auto yield_value(const remove_reference_t<yielded>& value)
				requires is_rvalue_reference_v<yielded>
						 && constructible_from<remove_cvref_t<yielded>, const remove_reference_t<yielded>&>
			{
				struct awaiter
				{
					remove_cvref_t<yielded> copy;

					bool await_ready() const noexcept
					{
						return false;
					}

					void await_suspend(coroutine_handle<promise_type> handle) noexcept
					{
						handle.promise().value_ = addressof(copy);
					}

					void await_resume() const noexcept
					{
					}
				};

				return awaiter {value};
			}

			// A generator body may not co_await
			void await_transform() = delete;

			void return_void() const noexcept
			{
			}

			void unhandled_exception()
			{
				throw;
			}

		private:
			friend iterator;

			add_pointer_t<yielded> value_ = nullptr;
		};

		generator(const generator&) = delete;

		generator(generator&& other) noexcept :
			handle_(exchange(other.handle_, {}))
		{
		}

		~generator()
		{
			if (handle_)
			{
				handle_.destroy();
			}
		}

		generator& operator=(generator other) noexcept
		{
			swap(handle_, other.handle_);
			return *this;
		}

		// Starts the coroutine. A generator can be traversed once.
		iterator begin()
		{
			handle_.resume();
			return iterator(handle_);
		}

		default_sentinel_t end() const noexcept
		{
			return default_sentinel;
		}

	private:
		class iterator
		{
		public:
			using value_type = value;
			using difference_type = ptrdiff_t;

			iterator(iterator&& other) noexcept :
				handle_(exchange(other.handle_, {}))
			{
			}

			iterator& operator=(iterator&& other) noexcept
			{
				handle_ = exchange(other.handle_, {});
				return *this;
			}

			reference operator*() const noexcept(is_nothrow_copy_constructible_v<reference>)
			{
				return static_cast<reference>(*handle_.promise().value_);
			}

			iterator& operator++()
			{
				handle_.resume();
				return *this;
			}

			void operator++(int)
			{
				++*this;
			}

			friend bool operator==(const iterator& it, default_sentinel_t)
			{
				return it.handle_.done();
			}

		private:
			friend generator;

			explicit iterator(coroutine_handle<promise_type> handle) noexcept :
				handle_(handle)
			{
			}

			coroutine_handle<promise_type> handle_;
		};

		explicit generator(coroutine_handle<promise_type> handle) noexcept :
			handle_(handle)
		{
		}

		coroutine_handle<promise_type> handle_;
	};

}

#endif
