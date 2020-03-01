#pragma once

#include "../Utils/std_types.hpp"

#include <iostream>
#include <random>
#include <vector>
#include <assert.h>


/*
template<typename T>
class GenerateSeed {
private:
    T mRNG;

public:
    GenerateSeed() = default; // default
    GenerateSeed(const T& x); // copy constructor
    GenerateSeed(T&& x); // move constructor

    template<typename Iterator>
    void generate(Iterator begin, Iterator end);
};

template <typename T>
GenerateSeed<T>::GenerateSeed(const T& x) : mRNG(x){} // copy

template <typename T>
GenerateSeed<T>::GenerateSeed(T&& x) : mRNG(x){} // move
*/


template<typename T>
class GenerateSeed {
private:
    T mRNG;

public:
    template<typename... Args>
    GenerateSeed(Args&&...args)
        : mRNG(std::forward<Args>(args)...) { }

    template<typename Iterator>
    void generate(Iterator begin, Iterator end) {
        std::uniform_int_distribution<u32> distribution;
        for (; begin != end; ++begin) {
            *begin = distribution(mRNG);
        }
    }
};


class RandomBatch {
private:
    std::vector<u16> mBatch;
    std::vector<u16>::iterator mCurrent;
    std::vector<u16>::iterator mNext;

public:
    RandomBatch(u16 amount);

    int intInRange(int lower_bound, int upper_bound);
    float floatInRange(float lower_bound, float upper_bound);

    void moveIterator();
    void resetIterator();

    void debugOutput();
    u16 getCurrentIterator() const;
    u16 getNextIterator() const;
};

namespace NDRNG {

    int intInRange(int lower_bound, int upper_bound);
    float floatInRange(float lower_bound, float upper_bound);
}
