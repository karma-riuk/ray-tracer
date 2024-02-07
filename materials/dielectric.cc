#include "dielectric.hpp"

bool dielectric::scatter(
    const ray& r_in, const hit& hit, color& attenuation, ray& scattered
) const {
    attenuation = color(1.0, 1.0, 1.0);
    double refraction_ratio = hit.front_face ? (1.0 / ir) : ir;

    vec3 unit_direction = unit_vector(r_in.direction());
    double cos_theta = fmin(dot(-unit_direction, hit.normal), 1.0);
    double sin_theta = sqrt(1 - cos_theta * cos_theta);

    bool cannot_refract = refraction_ratio * sin_theta > 1.0;

    vec3 direction =
        cannot_refract
                || reflectance(cos_theta, refraction_ratio) > random_double()
            ? reflect(unit_direction, hit.normal)
            : refract(unit_direction, hit.normal, refraction_ratio);

    scattered = ray(hit.p, direction);
    return true;
}
