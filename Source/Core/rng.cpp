#include "Core/rng.hpp"

#include "Core/profiler.hpp"

#include <climits>


/*----------------------------------------------------------------------------------*/

namespace {

    std::random_device rd;
    std::mt19937 generate(rd());

} // namespace


// For entire range of 's32' and 'f64'

s32 NDRNG::intInRange() {
    std::uniform_int_distribution<> distribution(std::numeric_limits<s32>::min(), std::numeric_limits<s32>::max());

    return distribution(::generate);
}
f32 NDRNG::floatInRange() {
    std::uniform_real_distribution<> distribution(std::numeric_limits<f32>::lowest(), std::numeric_limits<f32>::max());

    return distribution(::generate);
}


// For desired ranges

int NDRNG::intInRange(s64 lower_bound, s64 upper_bound) {
    std::uniform_int_distribution<> distribution(lower_bound, upper_bound);

    return distribution(::generate);
}
float NDRNG::floatInRange(f64 lower_bound, f64 upper_bound) {
    std::uniform_real_distribution<> distribution(lower_bound, upper_bound);

    return distribution(::generate);
}

/*----------------------------------------------------------------------------------*/