#pragma once

#include "../vec3.hpp"

struct hit {
    bool hit;
    point3 intersection;
    vec3 normal;
};
