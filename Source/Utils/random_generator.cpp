#include "Utils/random_generator.hpp"

#include <climits>


/*----------------------------------------------------------------------------------*/

namespace {

    std::mt19937 generate(GenerateSeedNDRNG<std::random_device>{});
}

/*----------------------------------------------------------------------------------*/

auto getGenerator(int seed) {
    auto generate = GenerateSeedDRNG(seed);
    return std::mt19937(generate);
}

/*----------------------------------------------------------------------------------*/

RandomBatch::RandomBatch(int amount)
    : mAmount(amount) { 

    for (size_t i = 0; i < amount; i++) {
        seeds.push_back(NDRNG::intInRange());
    }
}

int RandomBatch::intInRange(int lower_bound, int upper_bound) {
    return std::uniform_int_distribution<>{lower_bound, upper_bound}(getGenerator(seeds[mCurrent -1]));
}
float RandomBatch::floatInRange(float lower_bound, float upper_bound) {
    return std::uniform_real_distribution<>{lower_bound, upper_bound}(getGenerator(seeds[mCurrent - 1]));
}

void RandomBatch::moveForward() {
    mCurrent = (mCurrent == mAmount ? 1 : mCurrent + 1);
}
void RandomBatch::reset() {
    mCurrent = 1;
}

void RandomBatch::debugOutput() {
    for (size_t i = 0; i < mAmount; ++i) {
        std::cout << i << ',';
    }
}
u16 RandomBatch::getCurrentIndex() const {
    return mCurrent;
}
u16 RandomBatch::getNextIndex() const {
    return (mCurrent == mAmount ? 1 : mCurrent + 1);
}

/*----------------------------------------------------------------------------------*/

namespace NDRNG {

    // For entire range of 's32' and 'f64'

    s32 intInRange() {
        std::uniform_int_distribution<> distribution(std::numeric_limits<s32>::min(), std::numeric_limits<s32>::max());

        return distribution(::generate);
    }
    f32 floatInRange() {
        std::uniform_real_distribution<> distribution(std::numeric_limits<f32>::lowest(), std::numeric_limits<f32>::max());

        return distribution(::generate);
    }


    // For desired ranges

    int intInRange(int lower_bound, int upper_bound) {
        std::uniform_int_distribution<> distribution(lower_bound, upper_bound);

        return distribution(::generate);
    }
    float floatInRange(float lower_bound, float upper_bound) {
        std::uniform_real_distribution<> distribution(lower_bound, upper_bound);

        return distribution(::generate);
    }
}

/*----------------------------------------------------------------------------------*/