#pragma once

#include "../aabb.hpp"
#include "../interval.hpp"
#include "../ray.hpp"
#include "hit.hpp"

class object {
  public:
    virtual hit intersect(const ray& r, interval ray_t) const = 0;

    virtual aabb bounding_box() const = 0;

    virtual int calculate_depth() const = 0;

    virtual std::ostream& operator<<(std::ostream& out) const = 0;
};

// print the object
inline std::ostream& operator<<(std::ostream& out, const object& obj) {
    return obj.operator<<(out);
}
