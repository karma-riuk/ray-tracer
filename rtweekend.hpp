#pragma once

// Usings

#include <cmath>
#include <memory>

using std::make_shared;
using std::shared_ptr;
using std::sqrt;

// Constants

const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.141592653589793238462643383;
;

// Utility Functions

inline double degrees_to_radians(double degrees) {
    return degrees * pi / 180;
}

// Common Headers

#include "ray.hpp"
#include "vec3.hpp"
