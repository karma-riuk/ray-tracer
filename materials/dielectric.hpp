#include "material.hpp"

class dielectric : public material {
    double ir; // index of refraction

    static double reflectance(double cosine, double ref_idx) {
        // Use Schlick's approximation for reflectance.
        auto r0 = (1 - ref_idx) / (1 + ref_idx);
        r0 = r0 * r0;
        return r0 + (1 - r0) * pow((1 - cosine), 5);
    }

  public:
    dielectric(double ir): ir(ir) {}

    virtual bool scatter(
        const ray& r_in, const hit& rec, color& attenuation, ray& scattered
    ) const override;
};
