#pragma once

#include <atomic>
#include <cstddef>

class ClassID
{
public:
	ClassID() = delete;

	template<typename... Type>
	static size_t ID() noexcept;

private:
	static std::atomic<size_t> counter_;

	template<typename...>
	static size_t GenerateID() noexcept;
};

template<typename... Type>
size_t ClassID::ID() noexcept
{
	return GenerateID<std::decay_t<Type>...>();
}

template<typename...>
size_t ClassID::GenerateID() noexcept
{
	static const size_t value = counter_.fetch_add(1);
	return value;
}


template<typename T>
class TypeID
{

public:

	TypeID();


protected:

	uint32_t id_;

private:

	static std::atomic<uint32_t> counter_;

};


template<typename T>
TypeID<T>::TypeID():
	id_(++counter_)
{
}


template<typename T>
std::atomic<uint32_t> TypeID<T>::counter_ = 0;