// Implements a version of STD any that has additional features for allocation and SBO definition.
//	This implementation is C++23 compliant. A new standard will require a revisit of the type.

#pragma once

#include "Platform_Allocators.h"
#include "Types_Traits.h"

namespace Types
{
	namespace detail
	{

		static constexpr size_t DEFAULT_STACK_SIZE = 2;

		[[noreturn]] inline void HandleBadAnyCast()
		{
			Platform_AssertF(false, "@summary Bad Cast - https://en.cppreference.com/w/cpp/utility/any/bad_any_cast");
		};

	}

	/**
	 * \brief A std::any implementation that allows platform consistent SBO sizing and allocation hooks.
	 * \tparam Stack  - The size, in bytes, of the storage for the type
	 * \tparam Allocator - The allocator
	 *			TODO: C++ 20 replace with Allocator concept
	 */
	template<size_t Stack = detail::DEFAULT_STACK_SIZE, typename Allocator = Platform::StaticHeapAllocator<0, 0>>
	class Any
	{
	public:
		// Construction/Destruction
		constexpr Any() noexcept;
		Any(const Any& other);
		Any(Any&& other) noexcept;

		template<
			typename ValueType,
			std::enable_if_t<
				!std::is_same_v<std::decay_t<ValueType>, Any<Stack, Allocator>> &&
				!IsSpecializationV<std::decay_t<ValueType>, std::in_place_type_t> &&
				std::is_copy_constructible_v<std::decay_t<ValueType>>> = 0>
		Any(ValueType&& value);

		template<
			typename ValueType,
			typename... Args,
			std::enable_if_t<
				std::is_constructible_v<ValueType, Args...> && std::is_copy_constructible_v<std::decay_t<ValueType>>> =
				0>
		explicit Any(std::in_place_type_t<ValueType>, Args&&... args);

		template<
			typename ValueType,
			typename U,
			typename... Args,
			std::enable_if_t<
				std::is_constructible_v<std::decay_t<ValueType>, std::initializer_list<U>&, Args...> &&
				std::is_copy_constructible_v<std::decay_t<ValueType>>> = 0>
		explicit Any(std::in_place_type_t<ValueType>, std::initializer_list<U> il, Args&&... args);

		~Any();

		// Copy/Move
		Any& operator=(const Any& rhs);
		Any& operator=(Any&& rhs) noexcept;

		template<typename ValueType>
			requires !std::is_same_v<std::decay_t<ValueType>, Any<Stack, Allocator>> &&
				std::is_copy_constructible_v < std::decay_t < ValueType >>
		Any& operator=(ValueType&& rhs);

			// Modifiers
			template<class ValueType, class... Args>
			std::decay_t<ValueType>& Emplace(Args&&... args);
			template<class ValueType, class U, class... Args>
			std::decay_t<ValueType>& Emplace(std::initializer_list<U> il, Args&&... args);

			void Reset() noexcept;

			void Swap(Any& other) noexcept;

			// Observers
			[[nodiscard]] bool HasValue() const noexcept;
			[[nodiscard]] const std::type_info& Type() const noexcept;

		private:
			// NOTE: Types does not depend on EASTL, so std is used.
			using InternalStorageType = std::aligned_storage_t<Stack, std::alignment_of_v<void*>>;

			// std::variant is not used because storage type is known at compile
			union Storage
			{
				constexpr Storage()		= default;
				Storage(const Storage&) = delete;
				Storage(Storage&&)		= delete;
				Storage& operator=(const Storage&) = delete;
				Storage& operator=(Storage&&) = delete;

				void* externalStorage = nullptr;
				InternalStorageType internalStorage;
			};

			static constexpr void* EmptyGet(Storage* s, const void* info)
			{
				return nullptr;
			}

			static constexpr void EmptyCopy(Storage* dst, const Storage* src)
			{
			}

			static constexpr void EmptyMove(Storage* dst, Storage* src)
			{
			}

			static constexpr void EmptyDestroy(Storage* s)
			{
			}

			// Wrapper for the type erased data management
			class Actions
			{
				using Get	  = void* (*)(Storage* s, const void* type);
				using Copy	  = void (*)(Storage* dst, const Storage* src);
				using Move	  = void (*)(Storage* dst, Storage* src);
				using Destroy = void (*)(Storage* s);

			public:
				constexpr Actions() noexcept = default;

				constexpr Actions(Get g, Copy c, Move m, Destroy d, const void* t) noexcept :
					getFunction(g), copyFunction(c), moveFunction(m), destroyFunction(d), type(t)
				{
				}

				Get getFunction			= EmptyGet;
				Copy copyFunction		= EmptyCopy;
				Move moveFunction		= EmptyMove;
				Destroy destroyFunction = EmptyDestroy;

				const void* type = &typeid(void);
			};

			/**
			 * \brief Wraps the decision making for a given Type
			 * \tparam T - The type
			 */
			template<class T>
			class Traits
			{
				// Concept - if a type fits into the given storage size
				using UseInternalStorage = std::bool_constant<
					std::is_nothrow_move_constructible_v<T> && sizeof(T) <= sizeof(InternalStorageType) &&
					std::alignment_of_v<InternalStorageType> % std::alignment_of_v<T> == 0>;

				// Evaluation of Concept
				static constexpr bool Fits = UseInternalStorage::value;

				inline static bool CompareTypeID(const void* id)
				{
					return *static_cast<const std::type_info*>(id) == typeid(T);
				}

				template<std::enable_if_t<std::is_same_v<T, void>, int> = 0>
				inline static void* Get(Storage* s, const void* type)
				{
					return nullptr;
				}

				template<std::enable_if_t<Fits, int> = 0>
				inline static void* Get(Storage* s, const void* type)
				{
					if(CompareTypeID(type))
					{
						return static_cast<void*>(&s->buf);
					}
					return nullptr;
				}

				template<std::enable_if_t<!Fits, int> = 0>
				inline static void* Get(Storage* s, const void* type)
				{
					if(CompareTypeID(type))
					{
						return s->ptr;
					}
					return nullptr;
				}

				template<std::enable_if_t<std::is_same_v<T, void>, int> = 0>
				inline static void Copy(Storage* dst, const Storage* src)
				{
				}

				template<std::enable_if_t<Fits && std::is_nothrow_move_constructible_v<T>, int> = 0>
				inline static void Copy(Storage* dst, const Storage* src)
				{
					Traits::Make(
						dst,
						std::in_place_type_t<T>(),
						*static_cast<T const*>(static_cast<void const*>(&src->buf)));
				}
				template<std::enable_if_t<!Fits, int> = 0>
				inline static void Copy(Storage* dst, const Storage* src)
				{
					Traits::Make(
						dst,
						std::in_place_type_t<T>(),
						*static_cast<T const*>(static_cast<void const*>(src->ptr)));
				}

				template<std::enable_if_t<Fits, int> = 0>
				inline static void Move(Storage* dst, Storage* src)
				{
					Traits::Make(
						dst,
						std::in_place_type_t<T>(),
						std::move(*static_cast<T const*>(static_cast<void const*>(&src->buf))));
				}

				template<std::enable_if_t<!Fits, int> = 0>
				inline static void Move(Storage* dst, Storage* src)
				{
					dst->ptr = src->ptr;
				}

				template<std::enable_if_t<std::is_same_v<T, void>, int> = 0>
				inline static void Destroy(Storage* s)
				{
				}

				template<std::enable_if_t<std::is_trivially_destructible_v<T>, int> = 0>
				inline static void Destroy(Storage* s)
				{
				}

				template<std::enable_if_t<Fits && !std::is_trivially_destructible_v<T>, int> = 0>
				inline static void Destroy(Storage* s)
				{
					T& t = *static_cast<T*>(static_cast<void*>(const_cast<InternalStorageType*>(&s->buf)));
					t.~T();
				}

				template<std::enable_if_t<!Fits, int> = 0>
				inline static void Destroy(Storage* s)
				{
					delete static_cast<T*>(s->ptr);
				}

			public:
				template<class... Args, std::enable_if_t<Fits && std::is_nothrow_move_constructible_v<T>, int> = 0>
				inline static T& Make(Storage* s, std::in_place_type_t<T> vtype, Args&&... args)
				{
					return *(::new(static_cast<void*>(&s->buf)) T(std::forward<Args>(args)...));
				}

				template<class... Args, std::enable_if_t<!Fits, int> = 0>
				inline static T& Make(Storage* s, std::in_place_type_t<T> vtype, Args&&... args)
				{
					s->ptr = new T(std::forward<Args>(args)...);
					return *static_cast<T*>(s->ptr);
				}

				// The implemented table for the
				static constexpr Actions actions = Actions(Get<T>, Copy<T>, Move<T>, Destroy<T>, &typeid(T));

			private:
			};

			// VTable reference
			static constexpr Actions VoidActionsStorage		  = Actions();
			static constexpr const Actions* const VoidActions = &VoidActionsStorage;

			// This is the only free function that accesses state
			template<class ValueType>
			friend ValueType* AnyCast(Any* pAny) noexcept;

			// Handler - required to maintain type-checking and lifetime
			const Actions* actions_;

			// SBO/Heap ptr
			Storage storage_;
	};

	template<size_t Stack, typename Allocator>
	constexpr Any<Stack, Allocator>::Any() noexcept : actions_(VoidActions)
	{
	}

	template<size_t Stack, typename Allocator>
	Any<Stack, Allocator>::Any(const Any& other) : actions_(other.actions_)
	{
		actions_->copyFunction(&storage_, &other.storage_);
	}

	template<size_t Stack, typename Allocator>
	Any<Stack, Allocator>::Any(Any&& other) noexcept : actions_(other.actions_)
	{
		actions_->moveFunction(&storage_, &other.storage_);
		other.actions_ = VoidActions;
	}

	template<size_t Stack, typename Allocator>
	template<
		typename ValueType,
		std::enable_if_t<
			!std::is_same_v<std::decay_t<ValueType>, Any<Stack, Allocator>> &&
			!IsSpecializationV<std::decay_t<ValueType>, std::in_place_type_t> &&
			std::is_copy_constructible_v<std::decay_t<ValueType>>>>
	Any<Stack, Allocator>::Any(ValueType&& value) : actions_(&Traits<std::decay_t<ValueType>>::actions)
	{
		// TODO: Use concept come c++20. For now, these verify SFINAE
		static_assert(!std::is_same_v<std::decay_t<ValueType>, Any<Stack, Allocator>>, "Can't be any Any type");
		static_assert(
			!IsSpecializationV<std::decay_t<ValueType>, std::in_place_type_t>,
			"Can't be a specialization of std::in_place_type_t");
		static_assert(std::is_copy_constructible_v<std::decay_t<ValueType>>, "ValueType must be copy-constructible");

		Traits<ValueType>::Make(
			&storage_,
			std::in_place_type_t<ValueType>(),
			std::forward<std::decay_t<ValueType>>(value));
	}

	template<size_t Stack, typename Allocator>
	template<
		typename ValueType,
		typename... Args,
		std::enable_if_t<
			std::is_constructible_v<ValueType, Args...> && std::is_copy_constructible_v<std::decay_t<ValueType>>>>
	Any<Stack, Allocator>::Any(std::in_place_type_t<ValueType> vType, Args&&... args) :
		actions_(&Traits<ValueType>::actions)
	{
		// TODO: Use concept come c++20. For now, these verify SFINAE
		static_assert(std::is_constructible_v<ValueType, Args...>, "ValueType must be constructible with Args...");
		static_assert(std::is_copy_constructible_v<std::decay_t<ValueType>>, "ValueType must be copy-constructible");

		Traits<ValueType>::Make(&storage_, vType, std::forward<Args>(args)...);
	}

	template<size_t Stack, typename Allocator>
	template<
		typename ValueType,
		typename U,
		typename... Args,
		std::enable_if_t<
			std::is_constructible_v<std::decay_t<ValueType>, std::initializer_list<U>&, Args...> &&
			std::is_copy_constructible_v<std::decay_t<ValueType>>>>
	Any<Stack, Allocator>::Any(std::in_place_type_t<ValueType> vType, std::initializer_list<U> il, Args&&... args) :
		actions_(&Traits<ValueType>::actions)
	{
		// TODO: Use concept come c++20. For now, these verify SFINAE
		static_assert(
			std::is_constructible_v<std::decay_t<ValueType>, std::initializer_list<U>&, Args...>,
			"ValueType must be constructible with std::initializer_list<U>&, Args...>");
		static_assert(std::is_copy_constructible_v<std::decay_t<ValueType>>, "ValueType must be copy-constructible");

		Traits<ValueType>::Make(&storage_, vType, ValueType {il, std::forward<Args>(args)...});
	}

	template<size_t Stack, typename Allocator>
	Any<Stack, Allocator>::~Any()
	{
		actions_->destroyFunction(&storage_);
	}

	template<size_t Stack, typename Allocator>
	Any<Stack, Allocator>& Any<Stack, Allocator>::operator=(const Any& rhs)
	{
		if(this != &rhs)
		{
			actions_->destroyFunction(&storage_);
			actions_ = rhs.actions_;
			actions_->copyFunction(&storage_, &rhs.storage_);
		}
		return *this;
	}

	template<size_t Stack, typename Allocator>
	Any<Stack, Allocator>& Any<Stack, Allocator>::operator=(Any&& rhs) noexcept
	{
		if(this != &rhs)
		{
			actions_->destroyFunction(&storage_);
			actions_ = rhs.actions_;
			actions_->moveFunction(&storage_, &rhs.storage_);
			rhs.actions_ = VoidActions;
		}
		return *this;
	}

	template<size_t Stack, typename Allocator>
	template<
		typename ValueType,
		std::enable_if_t<
			!std::is_same_v<std::decay_t<ValueType>, Any<Stack, Allocator>> &&
			std::is_copy_constructible_v<std::decay_t<ValueType>>>>
	Any<Stack, Allocator>& Any<Stack, Allocator>::operator=(ValueType&& rhs)
	{
		// TODO: Use concept come c++20. For now, these verify SFINAE
		static_assert(!std::is_same_v<std::decay_t<ValueType>, Any<Stack, Allocator>>, "Can't be any Any type");
		static_assert(std::is_copy_constructible_v<std::decay_t<ValueType>>, "ValueType must be copy-constructible");

		*this = Any(std::forward<ValueType>(rhs));
		return *this;
	}

	template<size_t Stack, typename Allocator>
	template<class ValueType, class... Args>
	std::decay_t<ValueType>& Any<Stack, Allocator>::Emplace(Args&&... args)
	{
		// TODO: Use concept come c++20. For now, these verify SFINAE
		static_assert(
			std::is_constructible_v<std::decay_t<ValueType>, Args...>,
			"ValueType must be constructible with Args...");
		static_assert(std::is_copy_constructible_v<std::decay_t<ValueType>>, "ValueType must be copy-constructible");
	}

	template<size_t Stack, typename Allocator>
	template<class ValueType, class U, class... Args>
	std::decay_t<ValueType>& Any<Stack, Allocator>::Emplace(std::initializer_list<U> il, Args&&... args)
	{
		// TODO: Use concept come c++20. For now, these verify SFINAE
		static_assert(
			std::is_constructible_v<std::decay_t<ValueType>, std::initializer_list<U>&, Args...>,
			"ValueType must be constructible with std::initializer_list<U>&, Args...>");
		static_assert(std::is_copy_constructible_v<std::decay_t<ValueType>>, "ValueType must be copy-constructible");
	}

	template<size_t Stack, typename Allocator>
	void Any<Stack, Allocator>::Reset() noexcept
	{
		actions_->destroyFunction(&storage_);
		actions_ = VoidActions;
	}

	template<size_t Stack, typename Allocator>
	void Any<Stack, Allocator>::Swap(Any& other) noexcept
	{
		if(this == &other)
		{
			return;
		}

		Any tmp;
		other.actions_->move(&tmp.storage_, &other.storage_);
		actions_->move(&other.storage_, &storage_);
		other.actions_->move(&storage_, &tmp.storage_);

		std::swap(actions_, other.actions_);
	}

	template<size_t Stack, typename Allocator>
	bool Any<Stack, Allocator>::HasValue() const noexcept
	{
		return actions_ != VoidActions;
	}

	template<size_t Stack, typename Allocator>
	const std::type_info& Any<Stack, Allocator>::Type() const noexcept
	{
		return *static_cast<const std::type_info*>(actions_->type);
	}

	/**
	 * \brief A variation of std::any that errors if type is larger than the provided stack placement.
	 * \tparam Stack - The size, in bytes, of the storage for the type
	 */
	template<size_t Stack = detail::DEFAULT_STACK_SIZE>
	class StaticAny : public Any<Stack, void>
	{
	};

	//====================================================================================================
	// Types::Any Free Functions
	//====================================================================================================

	template<
		typename T,
		size_t Stack,
		typename Allocator,
		class U = std::remove_cv_t<std::remove_reference_t<T>>,
		std::enable_if_t<
			std::is_constructible_v<T, const U&> && std::is_constructible_v<T, U&> && std::is_constructible_v<T, U>> =
			0>
	T AnyCast(const Any<Stack, Allocator>& operand)
	{
		static_assert(std::is_constructible_v<T, const U&>);
		static_assert(std::is_constructible_v<T, U&>);
		static_assert(std::is_constructible_v<T, U>);

		auto* p = AnyCast<std::add_const_t<U>>(&operand);

		if(!p)
		{
			detail::HandleBadAnyCast();
		}

		return static_cast<T>(*p);
	}

	template<
		typename T,
		size_t Stack,
		typename Allocator,
		class U = std::remove_cv_t<std::remove_reference_t<T>>,
		std::enable_if_t<
			std::is_constructible_v<T, const U&> && std::is_constructible_v<T, U&> && std::is_constructible_v<T, U>> =
			0>
	T AnyCast(Any<Stack, Allocator>& operand)
	{
		static_assert(std::is_constructible_v<T, const U&>);
		static_assert(std::is_constructible_v<T, U&>);
		static_assert(std::is_constructible_v<T, U>);

		auto tmp = AnyCast<U>(&operand);
		if(!tmp)
		{
			detail::HandleBadAnyCast();
		}
		return static_cast<T>(*tmp);
	}

	template<
		typename T,
		size_t Stack,
		typename Allocator,
		class U = std::remove_cv_t<std::remove_reference_t<T>>,
		std::enable_if_t<
			std::is_constructible_v<T, const U&> && std::is_constructible_v<T, U&> && std::is_constructible_v<T, U>> =
			0>
	T AnyCast(Any<Stack, Allocator>&& operand)
	{
		static_assert(std::is_constructible_v<T, const U&>);
		static_assert(std::is_constructible_v<T, U&>);
		static_assert(std::is_constructible_v<T, U>);

		auto tmp = AnyCast<U>(&operand);
		if(!tmp)
		{
			detail::HandleBadAnyCast();
		}
		return static_cast<T>(std::move(*tmp));
	}

	template<
		typename T,
		size_t Stack,
		typename Allocator,
		class U = std::remove_cv_t<std::remove_reference_t<T>>,
		std::enable_if_t<
			std::is_constructible_v<T, const U&> && std::is_constructible_v<T, U&> && std::is_constructible_v<T, U>> =
			0>
	const T* AnyCast(const Any<Stack, Allocator>* operand) noexcept
	{
		static_assert(std::is_constructible_v<T, const U&>);
		static_assert(std::is_constructible_v<T, U&>);
		static_assert(std::is_constructible_v<T, U>);

		return AnyCast<T>(const_cast<Any*>(operand));
	}

	template<
		typename T,
		size_t Stack,
		typename Allocator,
		class U = std::remove_cv_t<std::remove_reference_t<T>>,
		std::enable_if_t<
			std::is_constructible_v<T, const U&> && std::is_constructible_v<T, U&> && std::is_constructible_v<T, U>> =
			0>
	T* AnyCast(Any<Stack, Allocator>* operand) noexcept
	{
		static_assert(std::is_constructible_v<T, const U&>);
		static_assert(std::is_constructible_v<T, U&>);
		static_assert(std::is_constructible_v<T, U>);

		if(operand && operand->has_value())
		{
			void* p = operand->actions_->get(
				&operand->storage_,
				&typeid(std::decay_t<T>)

			);
			return std::is_function<T> {} ? nullptr : static_cast<U*>(p);
		}
		return nullptr;
	}

	template<class T, size_t Stack, typename Allocator, class... Args>
	Any<Stack, Allocator> MakeAny(Args&&... args)
	{
		return Any(std::in_place_type<T>, std::forward<Args>(args)...);
	}
	template<class T, class U, size_t Stack, typename Allocator, class... Args>
	Any<Stack, Allocator> MakeAny(std::initializer_list<U> il, Args&&... args)
	{
		return Any(std::in_place_type<T>, il, std::forward<Args>(args)...);
	}
}
