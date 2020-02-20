#include "random_generator.h"


namespace random {

    int intInRange(int lower_bound, int upper_bound) {
        static std::random_device random;
        static std::mt19937 generate(random());
        std::uniform_int_distribution<> distribution(lower_bound, upper_bound);

        return distribution(generate);
    }

    float floatInRange(float lower_bound, float upper_bound) {
        static std::random_device random;
        static std::mt19937 generate(random());
        std::uniform_real_distribution<> distribution(lower_bound, upper_bound);

        return distribution(generate);
    }
}

