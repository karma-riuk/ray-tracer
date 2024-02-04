#include "sphere.hpp"

hit sphere::intersect(const ray& r, double t_min, double t_max) const {
    hit hit;

    hit.hit = false;
    vec3 oc = r.origin() - center;
    auto a = dot(r.direction(), r.direction());
    auto b = 2.0 * dot(oc, r.direction());
    auto c = dot(oc, oc) - radius * radius;
    auto delta = b * b - 4 * a * c;
    if (delta < 0)
        return hit;

    double t1 = (-b - sqrt(delta)) / (2 * a);
    double t2 = (-b + sqrt(delta)) / (2 * a);

    if ((t1 < t_min && t2 < t_min) || (t1 > t_max && t2 > t_max)
        || (t1 > t_max && t2 < t_min) || (t1 < t_min && t2 > t_max))
        return hit;
    hit.hit = true;

    double t = fmin( // find lowest positive number
        t1 < t_min ? t_max : t1,
        t2 < t_min ? t_max : t2
    );

    hit.t = t;
    hit.p = r.at(t);
    vec3 outwards_normal = (hit.p - center) / radius;
    hit.set_face_normal(r, outwards_normal);

    return hit;
}
