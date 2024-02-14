#include "material.hpp"

class metal : public material {
    color albedo;
    double fuzz;

  public:
    metal(const color& a): albedo(a), fuzz(0) {}

    metal(const color& a, double f): albedo(a), fuzz(f) {}

    virtual bool scatter(
        const ray& r_in, const hit& rec, color& attenuation, ray& scattered
    ) const override;

    std::ostream& operator<<(std::ostream& out) const override {
        out << "metal(" << albedo << ", " << fuzz << ")";
        return out;
    };
};
