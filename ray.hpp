#pragma once

#include "vec3.hpp"

class ray {
    point3 orig;
    vec3 dir;
    double tm;

  public:
    ray() {}

    ray(const point3& origin, const vec3& direction)
        : orig(origin),
          dir(direction),
          tm(0) {}

    ray(const point3& origin, const vec3& direction, double time = 0.0)
        : orig(origin),
          dir(direction),
          tm(time) {}

    point3 at(double t) const {
        return orig + t * dir;
    }

    point3 origin() const {
        return orig;
    }

    vec3 direction() const {
        return dir;
    }

    double time() const {
        return tm;
    }
};
