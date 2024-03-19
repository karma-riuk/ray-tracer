#pragma once

#include "../materials/material.hpp"
#include "object.hpp"

class sphere : public object {
    point3 center_point;
    vec3 center_vec;
    double radius;
    shared_ptr<material> mat;
    aabb bbox;
    bool is_moving;

  public:
    // Stationary Sphere
    sphere(point3 c, double r, shared_ptr<material> _mat)
        : center_point(c),
          radius(r),
          mat(_mat),
          is_moving(false) {
        auto rvec = vec3(r, r, r);
        bbox = aabb(center_point - rvec, center_point + rvec);
    }

    sphere(): center_point(0, 0, -1), radius(.5), mat(), is_moving(false) {}

    // Moving Sphere
    sphere(
        point3 _center1,
        point3 _center2,
        double _radius,
        shared_ptr<material> _material
    )
        : center_point(_center1),
          radius(_radius),
          mat(_material),
          is_moving(true) {
        center_vec = _center2 - _center1;
    }

    virtual hit intersect(const ray& r, interval ray_t) const override;
    virtual aabb bounding_box() const override;

    point3 sphere_center(double time) const {
        // Linearly interpolate from center1 to center2 according to time, where
        // t=0 yields center1, and t=1 yields center2.
        return center_point + time * center_vec;
    }

    virtual int calculate_depth() const override {
        return 1;
    }

    // print the sphere
    std::ostream& operator<<(std::ostream& out) const override {
        out << "sphere(" << center_point << ", " << radius << ", " << *mat
            << ")";
        return out;
    };
};
