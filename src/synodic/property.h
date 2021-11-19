#pragma once

#include <functional>

// TODO: Move somewhere appropriate
template<typename T>
class Property {

public:

	explicit Property(const T&);
	~Property() = default;

	Property& operator=(const T&);

	explicit operator T() const;

	void AddCallback(const std::function<void(T)>&);
	void Update();

private:

	T data_;
	std::vector<std::function<void(T)>> callbacks_;

};

template<typename T>
Property<T>::Property(const T& value) {
	data_ = value;
}


template<typename T>
Property<T>& Property<T>::operator=(const T& value){

	data_ = value;
	Update();

	return *this;

}

template<typename T>
void Property<T>::Update() {

	for (const auto& callback : callbacks_) {
		callback(data_);
	}

}

template<typename T>
Property<T>::operator T() const {

	return data_;

}

template<typename T>
void Property<T>::AddCallback(const std::function<void(T)>& fn) {

	callbacks_.push_back(fn);

}