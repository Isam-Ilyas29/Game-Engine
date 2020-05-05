#pragma once

#include "Core/std_types.hpp"
#include "Core/Assert/assert.hpp"

#include <random>
#include <algorithm>
#include <vector>


/*----------------------------------------------------------------------------------*/

// NDNRG

namespace NDRNG {

    // For entire range of 's32' and 'f64'
    s32 intInRange();
    f32 floatInRange();

    // For desired ranges
    int intInRange(s64 lower_bound, s64 upper_bound);
    float floatInRange(f64 lower_bound, f64 upper_bound);

} // namespace NDRNG

/*----------------------------------------------------------------------------------*/