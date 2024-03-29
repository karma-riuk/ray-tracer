#include "metal.hpp"

bool metal::scatter(
    const ray& r_in, const hit& hit, color& attenuation, ray& scattered
) const {
    vec3 reflected = reflect(unit_vector(r_in.direction()), hit.normal);
    scattered =
        ray(hit.p, reflected + fuzz * random_unit_vector(), r_in.time());
    attenuation = albedo;
    return (dot(scattered.direction(), hit.normal) > 0);
}
