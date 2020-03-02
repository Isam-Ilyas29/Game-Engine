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


// NDRNG

template<typename T>
class GenerateSeedNDRNG {
private:
    T mRNG;

public:
    template<typename... Args>
    GenerateSeedNDRNG(Args&&...args)
        : mRNG(std::forward<Args>(args)...) { }

    template<typename Iterator>
    void generate(Iterator begin, Iterator end) {
        std::uniform_int_distribution<u32> distribution;
        for (; begin != end; ++begin) {
            *begin = distribution(mRNG);
        }
    }
};

// DRNG

class GenerateSeedDRNG {
private:
    std::minstd_rand mRNG;

public:
    GenerateSeedDRNG(int i)
        : mRNG(i) {}

    template <typename T>
    void generate(T b, T e) {
        std::generate(b, e, [&]() {return std::uniform_int_distribution<u32>{}(mRNG); });
    }
};

/*----------------------------------------------------------------------------------*/

auto getGenerator(int i);

/*----------------------------------------------------------------------------------*/

class RandomBatch {
private:
    u16 mAmount;
    u16 mCurrent = 1;

public:
    RandomBatch(int amount);

    int intInRange(int lower_bound, int upper_bound);
    float floatInRange(float lower_bound, float upper_bound);

    void moveForward();
    void reset();

    void debugOutput();
    u16 getCurrentIndex() const;
    u16 getNextIndex() const;
};

/*----------------------------------------------------------------------------------*/

namespace NDRNG {

    int intInRange(int lower_bound, int upper_bound);
    float floatInRange(float lower_bound, float upper_bound);
}

/*----------------------------------------------------------------------------------*/
