#pragma once

#include "../ray.hpp"
#include "hit.hpp"

class object {
  public:
    virtual hit intersect(const ray& r) const = 0;
};

class sphere;
