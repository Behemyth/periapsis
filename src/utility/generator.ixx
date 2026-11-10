/*
std::generator (P2502R2) for standard libraries that do not provide <generator> yet.

When the standard library defines std::generator, this module exports nothing and the standard
type is used.

Not provided: allocator support. The Allocator parameter is accepted for source compatibility
and ignored.
 */

module;
#include <version>

export module generator;

import std;

#ifndef __cpp_lib_generator

namespace std
{

	// State shared by every generator that yields the same type, so that a generator can yield
	// the elements of a nested one. The outermost generator is the root.
	template<class Yielded>
	struct generator_polyfill_promise_base
	{
		// Root only: the most recently yielded object
		add_pointer_t<Yielded> value = nullptr;

		// Root only: the innermost coroutine, which is the one to resume
		coroutine_handle<> active;

		generator_polyfill_promise_base* root = this;

		// Nested only: the coroutine that yielded this generator's elements
		coroutine_handle<> parent;

		// Nested only: an exception to rethrow in the parent
		exception_ptr exception;

		// Returns to the parent when a nested generator finishes
		struct final_awaiter
		{
			bool await_ready() const noexcept
			{
				return false;
			}

			template<class Promise>
			coroutine_handle<> await_suspend(coroutine_handle<Promise> handle) const noexcept
			{
				generator_polyfill_promise_base& finished = handle.promise();
				if (finished.parent)
				{
					finished.root->active = finished.parent;
					return finished.parent;
				}
				return noop_coroutine();
			}

			void await_resume() const noexcept
			{
			}
		};
	};

	export template<class Ref, class V = void, class Allocator = void>
	class generator : public ranges::view_interface<generator<Ref, V, Allocator>>
	{
		using value = conditional_t<is_void_v<V>, remove_cvref_t<Ref>, V>;
		using reference = conditional_t<is_void_v<V>, Ref&&, Ref>;

		class iterator;

	public:
		using yielded = conditional_t<is_reference_v<reference>, reference, const reference&>;

		class promise_type : public generator_polyfill_promise_base<yielded>
		{
		public:
			generator get_return_object() noexcept
			{
				const auto handle = coroutine_handle<promise_type>::from_promise(*this);
				this->active = handle;
				return generator(handle);
			}

			suspend_always initial_suspend() const noexcept
			{
				return {};
			}

			auto final_suspend() const noexcept
			{
				return typename generator_polyfill_promise_base<yielded>::final_awaiter {};
			}

			// The yielded object outlives the suspension, so the iterator can refer to it
			suspend_always yield_value(yielded value) noexcept
			{
				this->root->value = addressof(value);
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
						handle.promise().root->value = addressof(copy);
					}

					void await_resume() const noexcept
					{
					}
				};

				return awaiter {value};
			}

			// Yields each element of a nested generator as if this generator had yielded it
			template<class Ref2, class V2, class Allocator2, class Unused>
				requires same_as<typename generator<Ref2, V2, Allocator2>::yielded, yielded>
			auto yield_value(ranges::elements_of<generator<Ref2, V2, Allocator2>&&, Unused> elements) noexcept
			{
				struct awaiter
				{
					generator<Ref2, V2, Allocator2> nested;

					bool await_ready() const noexcept
					{
						return false;
					}

					coroutine_handle<> await_suspend(coroutine_handle<promise_type> outer) noexcept
					{
						auto& inner = nested.handle_.promise();
						inner.root = outer.promise().root;
						inner.parent = outer;
						inner.root->active = nested.handle_;
						return nested.handle_;
					}

					void await_resume()
					{
						if (auto& inner = nested.handle_.promise(); inner.exception)
						{
							rethrow_exception(exchange(inner.exception, nullptr));
						}
					}
				};

				return awaiter {std::move(elements.range)};
			}

			// Yields each element of any other input range
			template<ranges::input_range Range, class Unused>
				requires convertible_to<ranges::range_reference_t<Range>, yielded>
			auto yield_value(ranges::elements_of<Range, Unused> elements)
			{
				auto nested = [](ranges::iterator_t<Range> current,
								  ranges::sentinel_t<Range> last) -> generator<yielded, ranges::range_value_t<Range>>
				{
					for (; current != last; ++current)
					{
						co_yield static_cast<yielded>(*current);
					}
				};

				return yield_value(
					ranges::elements_of(nested(ranges::begin(elements.range), ranges::end(elements.range))));
			}

			// A generator body may not co_await
			void await_transform() = delete;

			void return_void() const noexcept
			{
			}

			// A nested generator reports its exception to the generator that yielded it
			void unhandled_exception()
			{
				if (this->parent)
				{
					this->exception = current_exception();
				}
				else
				{
					throw;
				}
			}
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
		template<class, class, class>
		friend class generator;

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
				return static_cast<reference>(*handle_.promise().value);
			}

			iterator& operator++()
			{
				handle_.promise().active.resume();
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
