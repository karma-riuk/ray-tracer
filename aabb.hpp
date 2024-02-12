// Axis Aligned Bounding Box
#pragma once

#include "interval.hpp"
#include "ray.hpp"
#include "vec3.hpp"

class aabb {
  public:
    interval x, y, z;

    aabb() {}

    aabb(const interval& x, const interval& y, const interval& z)
        : x(x),
          y(y),
          z(z) {}

    aabb(const point3& a, const point3& b)
        : x(fmin(a.x(), b.x()), fmax(a.x(), b.x())),
          y(fmin(a.y(), b.y()), fmax(a.y(), b.y())),
          z(fmin(a.z(), b.z()), fmax(a.z(), b.z())) {}

    aabb(const aabb& a, const aabb& b): x(a.x, b.x), y(a.y, b.y), z(a.z, b.z) {}

    const interval& axis(int n) const {
        switch (n) {
        case 0:
            return x;
        case 1:
            return y;
        case 2:
            return z;
        default:
            throw std::runtime_error("Invalid axis");
        }
    }

    bool hit(const ray& r, interval ray_t) const {
        for (int a = 0; a < 3; a++) {
            auto invD = 1.0 / r.direction()[a];
            auto origin = r.origin()[a];

            auto t0 = (axis(a).min - origin) * invD;
            auto t1 = (axis(a).max - origin) * invD;

            if (invD < 0.0)
                std::swap(t0, t1);
            if (t0 > ray_t.min)
                ray_t.min = t0;
            if (t1 < ray_t.max)
                ray_t.max = t1;

            if (ray_t.max <= ray_t.min)
                return false;
        }
        return true;
    }
};
