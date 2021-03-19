#pragma once

class Structure{

public:

	Structure() = default;
	virtual ~Structure() = default;


};

class SparseStructure : public Structure{

public:

	SparseStructure() = default;
	virtual ~SparseStructure() = default;


};

template<class T>
class ExternalStructure : public Structure, public Contiguous {

public:

	ExternalStructure() = default;
	virtual ~ExternalStructure() = default;

	constexpr ExternalStructure<T>& operator=(const nonstd::span<T> span);

protected:

	nonstd::span<T> data;

};

template<class T>
constexpr ExternalStructure<T>& ExternalStructure<T>::operator=(const nonstd::span<T> span)
{
	data = span;
	return *this;
}
