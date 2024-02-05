#include "camera.hpp"

#include <fstream>

color camera::ray_color(const ray& r, const object& scene) const {
    hit hit = scene.intersect(r, interval(0, infinity));
    if (hit.hit)
        return 0.5 * (hit.normal + 1);

    vec3 unit_direction = unit_vector(r.direction());
    double t = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
}

void camera::initialize() {
    // Calculate the image height, and ensure that it's at least 1.
    image_height = aspect_ratio * image_width;
    image_height = image_height < 1 ? 1 : image_height;

    // Camera
    double focal_length = 1.;
    double viewport_height = 2.;
    double viewport_width =
        viewport_height * (static_cast<double>(image_width) / image_height);
    point3 camera_center = point3(0, 0, 0);

    // Calculate the vectors across the horizontal and down the verstical
    // viewport edges.
    vec3 viewport_u = vec3(viewport_width, 0, 0);
    vec3 viewport_v = vec3(0, -viewport_height, 0);

    // Calculate the hroizontal and vertical delta ectors from pixel to pixel.
    pixel_delta_u = viewport_u / image_width;
    pixel_delta_v = viewport_v / image_height;

    // Calculate the location of the upper left pixel.
    point3 viewport_upper_left = camera_center - viewport_u / 2 - viewport_v / 2
                               - vec3(0, 0, focal_length);

    pixel00_loc = viewport_upper_left + .5 * (pixel_delta_u + pixel_delta_v);
}

void progress_bar(double progress) {
    int barWidth = 70;

    std::cout << "[";
    int pos = barWidth * progress;
    for (int i = 0; i < barWidth; ++i)
        if (i < pos)
            std::cout << "=";
        else if (i == pos)
            std::cout << ">";
        else
            std::cout << " ";
    std::cout << "] " << int(progress * 100.0) << " %\r";
    std::cout.flush();
}

ray camera::get_ray(int i, int j) const {
    point3 pixel_center = pixel00_loc + i * pixel_delta_u + j * pixel_delta_v;
    point3 pixel_sample = pixel_center + pixel_sample_square();

    vec3 ray_direction = pixel_sample - camera_center;
    return ray(camera_center, ray_direction);
}

vec3 camera::pixel_sample_square() const {
    double px = -0.5 + random_double();
    double py = -0.5 + random_double();
    return (px * pixel_delta_u + py * pixel_delta_v);
}

void camera::render(const object& scene) {
    initialize();

    std::ofstream file(filename);
    file << "P3\n" << image_width << " " << image_height << "\n255\n";

    std::clog << "Writing to " << filename << std::endl;
    for (int j = 0; j < image_height; ++j) {
        for (int i = 0; i < image_width; ++i) {
            color pixel_color(0, 0, 0);
            for (int s = 0; s < samples_per_pixel; ++s)
                pixel_color += ray_color(get_ray(i, j), scene);
            pixel_color /= samples_per_pixel;

            double progress =
                (1. + i + j * image_width) / (image_width * image_height);
            progress_bar(progress);
            write_color(file, pixel_color);
        }
    }
    std::cout << std::endl;
    file.close();
}
