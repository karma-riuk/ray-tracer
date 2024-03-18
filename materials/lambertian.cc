#include "lambertian.hpp"

bool lambertian::scatter(
    const ray& r_in, const hit& hit, color& attenuation, ray& scattered
) const {
    vec3 scatter_direction = hit.normal + random_unit_vector();

    if (scatter_direction.near_zero())
        scatter_direction = hit.normal;

    scattered = ray(hit.p, scatter_direction, r_in.time());
    attenuation = albedo;
    return true;
}
