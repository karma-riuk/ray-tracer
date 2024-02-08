#pragma once

#include "color.hpp"
#include "objects/object.hpp"

class camera {
    int image_height;
    point3 camera_center;
    point3 pixel00_loc;
    vec3 pixel_delta_u;
    vec3 pixel_delta_v;
    vec3 u, v, w;
    vec3 defocus_disk_u, defocus_disk_v;

    void initialize();
    color ray_color(const ray& r, int depth, const object& world) const;
    ray get_ray(int i, int j) const;
    vec3 pixel_sample_square() const;
    point3 defocus_disk_sample() const;

  public:
    double aspect_ratio = 1;
    int image_width = 100;
    int samples_per_pixel = 10;
    std::string filename = "image.ppm";
    int max_depth = 10;

    double vfov = 90;

    point3 lookfrom = point3(0, 0, -1);
    point3 lookat = point3(0, 0, 0);
    vec3 vup = vec3(0, 1, 0);

    double defocus_angle = 0;
    double focus_dist = 10;

    void render(const object& scene);
};
