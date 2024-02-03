#pragma once

#include "../vec3.h"

struct hit {
    bool hit;
    point3 intersection;
    vec3 normal;
};
