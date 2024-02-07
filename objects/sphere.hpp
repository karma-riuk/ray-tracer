#pragma once

#include "object.hpp"

class sphere : public object {
    point3 center;
    double radius;
    shared_ptr<material> mat;

  public:
    sphere(point3 c, double r, shared_ptr<material> _mat)
        : center(c),
          radius(r),
          mat(_mat) {}

    sphere(): center(0, 0, -1), radius(.5), mat() {}

    virtual hit intersect(const ray& r, interval ray_t) const override;
};
