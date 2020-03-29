#pragma once

#include "Core/std_types.hpp"
#include "Core/Assert/assert.hpp"

#include <iostream>
#include <random>
#include <vector>
#include <cassert>


class GenerateSeedDRNG {
private:
    std::minstd_rand mRNG;

public:
    GenerateSeedDRNG(int seed)
        : mRNG(seed) {}

    template <typename T>
    void generate(T b, T e) {
        std::generate(b, e, [&]() {return std::uniform_int_distribution<u32>{}(mRNG); });
    }
};

/*----------------------------------------------------------------------------------*/

auto getGenerator(int seed);

/*----------------------------------------------------------------------------------*/

class RandomBatch {
private:
    u16 mAmount;
    u16 mCurrent = 1;
    
    std::vector<s64> seeds;

public:
    RandomBatch(u16 amount);

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

    // For entire range of 's32' and 'f64'
    s32 intInRange();
    f32 floatInRange();

    // For desired ranges
    int intInRange(s64 lower_bound, s64 upper_bound);
    float floatInRange(f64 lower_bound, f64 upper_bound);
}

/*----------------------------------------------------------------------------------*/
