#pragma once

#include "../aabb.hpp"
#include "../interval.hpp"
#include "../ray.hpp"
#include "hit.hpp"

class object {
  public:
    virtual hit intersect(const ray& r, interval ray_t) const = 0;

    virtual aabb bounding_box() const = 0;
};
