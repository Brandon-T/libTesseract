#ifndef PASCALARRAY_HPP_INCLUDED
#define PASCALARRAY_HPP_INCLUDED

#include <iostream>
#include <cstdint>

template<typename T>
class PascalArray
{
	private:
		char* data;
		T* ptr;
		std::int32_t _size;

	public:
		PascalArray(std::size_t n) :
						data(new char[(n * sizeof(T)) + (sizeof(std::int32_t) * 2)]),
						ptr(reinterpret_cast<T*>(&data[sizeof(std::int32_t) * 2])), _size(n)
						{
						    *reinterpret_cast<std::int32_t*>(data) = -1;
						    *reinterpret_cast<std::int32_t*>(&data[sizeof(std::int32_t)]) = _size - 1;
						}

        PascalArray(const PascalArray& other) = delete;
        PascalArray(PascalArray &&other);
		~PascalArray() {delete[] data;}

		T& operator[](std::int32_t index) {return ptr[index];}
		const T& operator[](std::int32_t index) const {return ptr[index];}

		PascalArray& operator = (const PascalArray &other) = delete;
		PascalArray& operator = (PascalArray&& other);

		std::size_t size() const {return _size;}
};


template<typename T>
PascalArray<T>::PascalArray(PascalArray<T> &&other) : data(std::move(other.data)), ptr(std::move(other.ptr)), _size(std::move(other._size))
{
    other.data = nullptr;
    other.ptr = nullptr;
    other._size = 0;
}

template<typename T>
PascalArray<T>& PascalArray<T>::operator = (PascalArray&& other)
{
    data = std::move(other.data);
    ptr = std::move(other.ptr);
    _size = std::move(other._size);
    other.data = nullptr;
    other.ptr = nullptr;
    other._size = 0;
    return *this;
}


template<typename T>
int PascalHigh(T* Arr)
{
    return reinterpret_cast<int*>(Arr)[-1];
}

template<typename T>
int PascalLength(T* Arr)
{
    return reinterpret_cast<int*>(Arr)[-1] + 1;
}

template<typename T>
int PascalReferenceCount(T* Arr)
{
    return reinterpret_cast<int*>(Arr)[-2];
}

#endif // PASCALARRAY_HPP_INCLUDED
