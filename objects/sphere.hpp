#pragma once

#include "objects.hpp"

class sphere : public object {
    point3 center;
    double radius;

  public:
    sphere(point3 c, double r): center(c), radius(r) {}

    sphere(): center(0, 0, -1), radius(.5) {}

    hit intersect(const ray& r) const;
};
