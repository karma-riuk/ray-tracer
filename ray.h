#pragma once

#include "vec3.h"

class ray {
    point3 orig;
    vec3 dir;

  public:
    ray() {}

    ray(const point3& origin, const vec3& direction)
        : orig(origin),
          dir(direction) {}

    point3 at(double t) const {
        return orig + t * dir;
    }

    point3 origin() const {
        return orig;
    }

    vec3 direction() const {
        return dir;
    }
};
