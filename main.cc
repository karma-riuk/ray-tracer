#include "bvh_node.hpp"
#include "camera.hpp"
#include "materials/dielectric.hpp"
#include "materials/lambertian.hpp"
#include "materials/metal.hpp"
#include "objects/object.hpp"
#include "objects/object_list.hpp"
#include "objects/sphere.hpp"
#include "profiler.hpp"

#define PROFILE_SCOPE(label)                                                   \
    std::unique_ptr<profiler> _;                                               \
    if (profile)                                                               \
        _ = std::make_unique<profiler>(label);

int main(int argc, char* argv[]) {
    object_list scene;

    auto ground_material = make_shared<lambertian>(color(0.5, 0.5, 0.5));
    scene.add(make_shared<sphere>(point3(0, -1000, 0), 1000, ground_material));

    int N = 10;
    bool verbose = false;
    bool profile = false;
    bool apply_bvh = false;
    bvh_node::LEAF_SIZE = 1;

    camera cam;
    // Parse command line arguments
    for (int i = 1; i < argc; i++)
        if (std::string(argv[i]) == "-n")
            N = std::stoi(argv[++i]);
        else if (std::string(argv[i]) == "-v")
            verbose = true;
        else if (std::string(argv[i]) == "-p")
            profile = true;
        else if (std::string(argv[i]) == "-l")
            bvh_node::LEAF_SIZE = std::stoi(argv[++i]);
        else if (std::string(argv[i]) == "-w")
            cam.image_width = std::stoi(argv[++i]);
        else if (std::string(argv[i]) == "-bvh")
            apply_bvh = true;

    for (int a = -N; a < N; a++) {
        for (int b = -N; b < N; b++) {
            auto choose_mat = random_double();
            point3 center(
                a + 0.9 * random_double(),
                0.2,
                b + 0.9 * random_double()
            );

            if ((center - point3(4, 0.2, 0)).length() > 0.9) {
                shared_ptr<material> sphere_material;

                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = color::random() * color::random();
                    sphere_material = make_shared<lambertian>(albedo);
                    scene.add(make_shared<sphere>(center, 0.2, sphere_material)
                    );
                } else if (choose_mat < 0.95) {
                    // metal
                    auto albedo = color::random(0.5, 1);
                    auto fuzz = random_double(0, 0.5);
                    sphere_material = make_shared<metal>(albedo, fuzz);
                    scene.add(make_shared<sphere>(center, 0.2, sphere_material)
                    );
                } else {
                    // glass
                    sphere_material = make_shared<dielectric>(1.5);
                    scene.add(make_shared<sphere>(center, 0.2, sphere_material)
                    );
                }
            }
        }

        auto material1 = make_shared<dielectric>(1.5);
        scene.add(make_shared<sphere>(point3(0, 1, 0), 1.0, material1));

        auto material2 = make_shared<lambertian>(color(0.4, 0.2, 0.1));
        scene.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));

        auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
        scene.add(make_shared<sphere>(point3(4, 1, 0), 1.0, material3));
    }

    if (apply_bvh) {
        // PROFILE_SCOPE("");
        scene = object_list(make_shared<bvh_node>(scene));
    }

    cam.aspect_ratio = 9. / 16.;
    cam.samples_per_pixel = 32;
    cam.max_depth = 8;

    cam.vfov = 20;
    cam.lookfrom = point3(13, 2, 3);
    cam.lookat = point3(0, 0, 0);
    cam.vup = vec3(0, 1, 0);

    cam.defocus_angle = 0.6;
    cam.focus_dist = 10.0;

    {
        PROFILE_SCOPE("");
        cam.render(scene, verbose);
    }
    std::cerr << scene.calculate_depth() << "\n";
}
