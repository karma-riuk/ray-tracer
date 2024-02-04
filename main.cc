#include "color.hpp"
#include "objects/sphere.hpp"
#include "ray.hpp"
#include "vec3.hpp"

#include <fstream>
#include <iostream>
#include <vector>

color ray_color(const ray& r) {
    vec3 unit_direction = unit_vector(r.direction());
    double t = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
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

object_list generate_scene() {
    object_list objects;
    objects.add(std::shared_ptr<object>(new sphere()));
    return objects;
}

int main() {
    // Image
    double aspect_ratio = 9. / 16.;
    int image_width = 400;

    // Calculate the image height, and ensure that it's at least 1.
    int image_height = aspect_ratio * image_width;
    image_height = image_height < 1 ? 1 : image_height;

    // Camera
    double focal_length = 1.;
    double viewport_height = 2.;
    double viewport_width =
        viewport_height * (static_cast<double>(image_width) / image_height);
    std::cout << "viewport_width: " << viewport_width << std::endl;
    std::cout << "viewport_height: " << viewport_height << std::endl;
    point3 camera_center = point3(0, 0, 0);

    // Calculate the vectors across the horizontal and down the verstical
    // viewport edges.
    vec3 viewport_u = vec3(viewport_width, 0, 0);
    vec3 viewport_v = vec3(0, -viewport_height, 0);

    // Calculate the hroizontal and vertical delta ectors from pixel to pixel.
    vec3 pixel_delta_u = viewport_u / image_width;
    vec3 pixel_delta_v = viewport_v / image_height;

    // Calculate the location of the upper left pixel.
    point3 viewport_upper_left = camera_center - viewport_u / 2 - viewport_v / 2
                               - vec3(0, 0, focal_length);
    point3 pixel00_loc =
        viewport_upper_left + .5 * (pixel_delta_u + pixel_delta_v);


    std::string filename = "image.ppm";
    std::ofstream file(filename);

    // Get the scene
    object_list scene = generate_scene();

    // Render
    file << "P3\n" << image_width << " " << image_height << "\n255\n";

    std::clog << "Writing to " << filename << std::endl;
    for (int j = 0; j < image_height; ++j) {
        for (int i = 0; i < image_width; ++i) {
            point3 pixel_center =
                pixel00_loc + i * pixel_delta_u + j * pixel_delta_v;
            vec3 ray_direction = unit_vector(pixel_center - camera_center);

            ray r = ray(camera_center, ray_direction);
            color pixel_color = ray_color(r);

            hit hit = scene.intersect(r, 0.6, 1.4);
            if (hit.hit)
                pixel_color = 0.5 * (hit.normal + 1);

            double progress =
                (1. + i + j * image_width) / (image_width * image_height);
            progress_bar(progress);
            write_color(file, pixel_color);
        }
    }
    std::cout << std::endl;
    file.close();
}
