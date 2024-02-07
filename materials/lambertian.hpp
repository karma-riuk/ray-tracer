#include "material.hpp"

class lambertian : public material {
    color albedo;

  public:
    lambertian(const color& a): albedo(a) {}

    virtual bool scatter(
        const ray& r_in, const hit& rec, color& attenuation, ray& scattered
    ) const override;
};
