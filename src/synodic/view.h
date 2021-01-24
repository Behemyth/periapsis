#pragma once

#include <span>

#include <functional>
#include <optional>


template<class T>
class View {

	using size_type = size_t;

	// TODO: std::unique_function C++20
	using function_type = std::function<size_type(size_type)>;

public:

	constexpr View();
	constexpr View(T* ptr, size_type count);
	virtual ~View() = default;

	T& operator[](size_type);

	void Map(function_type&& function);


private:

	std::optional<function_type> indexMap_;

	std::span<T> data_;


};

template<class T>
constexpr View<T>::View(): data_(nullptr, size_)
{
}

template<class T>
constexpr View<T>::View(T* ptr, size_type count): data_(ptr, count)
{
}

template<class T>
T& View<T>::operator[](size_type index)
{
	if (!indexMap_.has_value()) {
		return data_[index];
	}
	
	return data_[*indexMap_(index)];

}

template<class T>
void View<T>::Map(function_type&& function)
{

	indexMap_.emplace(std::move(function));

}