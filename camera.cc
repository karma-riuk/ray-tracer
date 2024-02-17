#include "camera.hpp"

#include "materials/material.hpp"

#include <atomic>
#include <fstream>
#include <mutex>
#include <thread>
#include <vector>

color camera::ray_color(const ray& r, int depth, const object& scene) const {
    if (depth <= 0)
        return color(0, 0, 0);

    hit hit = scene.intersect(r, interval(0.001, infinity));
    if (hit.hit) {
        ray scattered;
        color attenuation;
        if (hit.mat->scatter(r, hit, attenuation, scattered))
            return attenuation * ray_color(scattered, depth - 1, scene);
        return color(0, 0, 0);
    }

    vec3 unit_direction = unit_vector(r.direction());
    double t = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
}

void camera::initialize() {
    // Calculate the image height, and ensure that it's at least 1.
    image_height = aspect_ratio * image_width;
    image_height = image_height < 1 ? 1 : image_height;

    camera_center = lookfrom;

    // Camera
    double theta = degrees_to_radians(vfov);
    double h = tan(theta / 2.);
    double viewport_height = 2 * h * focus_dist;
    double viewport_width =
        viewport_height * (static_cast<double>(image_width) / image_height);

    // Calculate the orthonormal basis for the camera.
    w = unit_vector(lookfrom - lookat);
    u = unit_vector(cross(vup, w));
    v = cross(w, u);

    // Calculate the vectors across the horizontal and down the verstical
    // viewport edges.
    vec3 viewport_u = viewport_width * u;
    vec3 viewport_v = viewport_height * -v;

    // Calculate the hroizontal and vertical delta ectors from pixel to pixel.
    pixel_delta_u = viewport_u / image_width;
    pixel_delta_v = viewport_v / image_height;

    // Calculate the location of the upper left pixel.
    point3 viewport_upper_left =
        camera_center - (focus_dist * w) - .5 * (viewport_u + viewport_v);

    pixel00_loc = viewport_upper_left + .5 * (pixel_delta_u + pixel_delta_v);

    // Calculate the camera defocus disk basis vectors.
    double defocus_radius =
        focus_dist * tan(degrees_to_radians(defocus_angle / 2));
    defocus_disk_u = defocus_radius * u;
    defocus_disk_v = defocus_radius * v;
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

    point3 origin = defocus_angle <= 0 ? camera_center : defocus_disk_sample();
    vec3 direction = pixel_sample - origin;

    return ray(origin, direction);
}

point3 camera::defocus_disk_sample() const {
    auto p = random_in_unit_disk();
    return camera_center + (p.x() * defocus_disk_u) + (p.y() * defocus_disk_v);
}

vec3 camera::pixel_sample_square() const {
    double px = -0.5 + random_double();
    double py = -0.5 + random_double();
    return (px * pixel_delta_u + py * pixel_delta_v);
}

void camera::render(const object& scene) {
    initialize();
    std::vector<color> image(image_width * image_height);

    std::ofstream file(filename);
    file << "P3\n" << image_width << " " << image_height << "\n255\n";

    std::clog << "Writing to " << filename << std::endl;

    int pixel_window = 100; // each thread will render this many pixels before
                            // getting the new window
    std::atomic<int> progress(0);
    std::atomic<int> next_pixel_index(0);
    std::mutex progress_mutex;

    auto render_thread = [&](int thread_id) {
        int start_pixel;
        while ((start_pixel = next_pixel_index.fetch_add(pixel_window))
               < image_width * image_height) {
            for (int pixel_index = start_pixel;
                 pixel_index < image_width * image_height
                 && pixel_index < start_pixel + pixel_window;
                 pixel_index++) {
                int i = pixel_index % image_width;
                int j = pixel_index / image_width;

                color pixel_color(0, 0, 0);
                for (int s = 0; s < samples_per_pixel; ++s)
                    pixel_color += ray_color(get_ray(i, j), max_depth, scene);
                pixel_color /= samples_per_pixel;

                image[pixel_index] = pixel_color;

                std::lock_guard<std::mutex> lock(progress_mutex);
                progress_bar(double(progress++) / (image_width * image_height));
            }
        }
    };

    // Launch threads
    const int num_threads = std::thread::hardware_concurrency() - 1;
    // const int num_threads = 1;
    std::vector<std::thread> threads;
    for (int t = 0; t < num_threads; ++t)
        threads.emplace_back(render_thread, t);

    // Wait for all threads to finish
    for (auto& thread : threads)
        thread.join();

    // Write the image to file
    for (int j = 0; j < image_height; ++j)
        for (int i = 0; i < image_width; ++i)
            write_color(file, image[j * image_width + i]);

    std::cout << std::endl;
    file.close();
}
