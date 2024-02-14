#pragma once

#include "object.hpp"

class sphere : public object {
    point3 center;
    double radius;
    shared_ptr<material> mat;
    aabb bbox;

  public:
    sphere(point3 c, double r, shared_ptr<material> _mat)
        : center(c),
          radius(r),
          mat(_mat) {
        auto rvec = vec3(r, r, r);
        bbox = aabb(center - rvec, center + rvec);
    }

    sphere(): center(0, 0, -1), radius(.5), mat() {}

    virtual hit intersect(const ray& r, interval ray_t) const override;
    virtual aabb bounding_box() const override;

    // print the sphere
    std::ostream& print(std::ostream& out) const override {
        out << "sphere(" << center << ", " << radius << ")";
        return out;
    };
};
