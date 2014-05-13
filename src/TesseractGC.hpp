#ifndef TESSERACTGC_HPP_INCLUDED
#define TESSERACTGC_HPP_INCLUDED

#include <algorithm>
#include <tuple>
#include <vector>

template<typename T>
class TesseractGC
{
    private:
        std::vector<T> allocations;

    public:
        typedef typename std::vector<T>::iterator iterator;
        typedef const iterator                    const_iterator;
        typedef T                                 value_type;

        TesseractGC() {};
        ~TesseractGC();

        iterator begin() {return allocations.begin();}
        iterator end() {return allocations.end();}
        const_iterator begin() const {return allocations.begin();}
        const_iterator end() const {return allocations.end();}

        iterator erase(iterator it) {return allocations.erase(it);}

        T* push_back(T&& other);
};

template<typename T>
TesseractGC<T>::~TesseractGC()
{
    allocations.clear();
}

template<typename T>
T* TesseractGC<T>::push_back(T&& other)
{
    allocations.emplace_back(std::forward<T&&>(other));
    return &allocations.back();
}

#endif // TESSERACTGC_HPP_INCLUDED
