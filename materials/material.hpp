#pragma once


#include "../color.hpp"
#include "../objects/hit.hpp"
#include "../ray.hpp"

struct material {
    virtual bool scatter(
        const ray& r_in, const hit& rec, color& attenuation, ray& scattered
    ) const = 0;
};
