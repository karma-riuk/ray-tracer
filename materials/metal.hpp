#include "material.hpp"

class metal : public material {
    color albedo;

  public:
    metal(const color& a): albedo(a) {}

    virtual bool scatter(
        const ray& r_in, const hit& rec, color& attenuation, ray& scattered
    ) const override;
};
