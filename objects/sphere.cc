#include "sphere.hpp"

hit sphere::intersect(const ray& r, interval ray_t) const {
    hit hit;

    hit.hit = false;
    vec3 oc = r.origin() - center;
    auto a = dot(r.direction(), r.direction());
    auto b = 2.0 * dot(oc, r.direction());
    auto c = dot(oc, oc) - radius * radius;
    auto delta = b * b - 4 * a * c;
    if (delta < 0)
        return hit;

    double t = (-b - sqrt(delta)) / (2 * a);
    if (!ray_t.surrounds(t)) {
        t = (-b + sqrt(delta)) / (2 * a);
        if (!ray_t.surrounds(t))
            return hit;
    }
    hit.hit = true;

    hit.t = t;
    hit.p = r.at(t);
    vec3 outwards_normal = (hit.p - center) / radius;
    hit.set_face_normal(r, outwards_normal);
    hit.mat = mat;

    return hit;
}
