#include "random_generator.hpp"

#include "std_types.hpp"


/*----------------------------------------------------------------------------------*/

namespace {

    std::mt19937 generate(GenerateSeed<std::random_device>{});
}

/*----------------------------------------------------------------------------------*/

RandomBatch::RandomBatch(u16 amount) {
    assert(amount >= 1);

    for (u16 i = 1; i < amount + 1; i++) {
        mBatch.push_back(i);
    }

    mCurrent = mBatch.begin();
    mNext = mBatch.begin() + 1;
}

int RandomBatch::intInRange(int lower_bound, int upper_bound) {
    std::mt19937 generate_int(GenerateSeed<std::minstd_rand>(static_cast<int>(*mCurrent)));
    std::uniform_int_distribution<> distribution(lower_bound, upper_bound);

    return distribution(generate_int);
}
float RandomBatch::floatInRange(float lower_bound, float upper_bound) {
    std::mt19937 generate_float(GenerateSeed<std::minstd_rand>(static_cast<int>(*mCurrent)));
    std::uniform_real_distribution<> distribution(lower_bound, upper_bound);

    return distribution(generate_float);
}

void RandomBatch::moveIterator() {
    if (mCurrent + 1 != mBatch.end()) {
        mCurrent++;
    }
    else {
        mCurrent = mBatch.begin();
    }

    if (mNext != mBatch.end() - 1) {
        mNext++;
    }
    else {
        mNext = mBatch.begin();
    }
}
void RandomBatch::resetIterator() {
    mCurrent = mBatch.begin();
    mNext = mBatch.begin() + 1;
}

// Debug functions

u16 RandomBatch::getCurrentIterator() const {
    return *mCurrent;
}
u16 RandomBatch::getNextIterator() const {
    return *mNext;
}

void RandomBatch::debugOutput() {
    std::cout << "\n";
    for (auto it = mBatch.begin(); it != mBatch.end(); it++) {
        std::cout << *it << ", ";
    }
    std::cout << "\n";
}

/*----------------------------------------------------------------------------------*/

namespace NDRNG {

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
