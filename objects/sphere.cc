#include "sphere.hpp"

hit sphere::intersect(const ray& r) const {
    hit hit;

    hit.hit = false;
    vec3 oc = r.origin() - center;
    auto a = dot(r.direction(), r.direction());
    auto b = 2.0 * dot(oc, r.direction());
    auto c = dot(oc, oc) - radius * radius;
    auto delta = b * b - 4 * a * c;
    if (delta < 0)
        return hit;

    double t1 = (-b + sqrt(delta)) / (2 * a);
    double t2 = (-b - sqrt(delta)) / (2 * a);

    if (t1 < 0 && t2 < 0)
        return hit;
    hit.hit = true;

    float t = fmin( // find lowest positive number
        t1 < 0 ? INFINITY : t1,
        t2 < 0 ? INFINITY : t2
    );

    hit.intersection = r.at(t);
    hit.normal = (hit.intersection - center) / radius;

    return hit;
}
