#pragma once

#include "../ray.hpp"
#include "../vec3.hpp"

struct material;

struct hit {
    double t;
    bool hit = false;
    point3 p;
    vec3 normal;
    shared_ptr<material> mat;

    bool front_face;

    void set_face_normal(const ray& r, const vec3& outward_normal) {
        front_face = dot(r.direction(), outward_normal) < 0;
        normal = front_face ? outward_normal : -outward_normal;
    }
};
