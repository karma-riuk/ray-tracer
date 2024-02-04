#pragma once

#include "../ray.hpp"
#include "hit.hpp"

class object {
  public:
    virtual hit intersect(const ray& r, double t_min, double t_max) const = 0;
};
