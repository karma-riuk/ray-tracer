#pragma once

#include "object.hpp"

class sphere : public object {
    point3 center;
    double radius;

  public:
    sphere(point3 c, double r): center(c), radius(r) {}

    sphere(): center(0, 0, -1), radius(.5) {}

    virtual hit
    intersect(const ray& r, double t_min, double t_max) const override;
};
