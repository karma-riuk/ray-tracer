#pragma once

#include "color.hpp"
#include "objects/object.hpp"
#include "rtweekend.hpp"

class camera {
    int image_height;
    point3 camera_center;
    point3 pixel00_loc;
    vec3 pixel_delta_u;
    vec3 pixel_delta_v;

    void initialize();
    color ray_color(const ray& r, const object& world) const;

  public:
    double aspect_ratio = 1;
    int image_width = 100;
    std::string filename = "image.ppm";

    void render(const object& scene);
};
