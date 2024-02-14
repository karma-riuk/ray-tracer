#include "bvh_node.hpp"
#include "camera.hpp"
#include "materials/dielectric.hpp"
#include "materials/lambertian.hpp"
#include "materials/metal.hpp"
#include "objects/object.hpp"
#include "objects/object_list.hpp"
#include "objects/sphere.hpp"

int main() {
    object_list scene;

#ifdef RANDOM_SCENE
    auto material_ground = make_shared<lambertian>(color(.5, .5, .5));
    scene.add(
        std::make_shared<sphere>(point3(0, -1000, 0), 1000, material_ground)
    );

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            double choose_mat = random_double();
            point3 center(
                a + 0.9 * random_double(),
                0.2,
                b + 0.9 * random_double()
            );

            if ((center - point3(4, 0.2, 0)).length() > 0.9) {
                double radius = random_double(.05, .3);
                center[1] = radius;
                shared_ptr<material> sphere_material;

                if (choose_mat < 0.8) {
                    // Diffuse
                    color albedo = color::random() * color::random();
                    sphere_material = make_shared<lambertian>(albedo);
                    scene.add(std::make_shared<sphere>(
                        center,
                        radius,
                        sphere_material
                    ));
                } else if (choose_mat < 0.95) {
                    // Metal
                    color albedo = color::random(.5, 1);
                    double fuzz = random_double(0, .5);
                    sphere_material = make_shared<metal>(albedo, fuzz);
                    scene.add(std::make_shared<sphere>(
                        center,
                        radius,
                        sphere_material
                    ));
                } else {
                    // Glass
                    sphere_material = make_shared<dielectric>(1.5);
                    scene.add(std::make_shared<sphere>(
                        center,
                        radius,
                        sphere_material
                    ));
                }
            }
        }
    }
#endif

    auto material1 = make_shared<lambertian>(color(.9, .1, .1));
    auto material2 = make_shared<lambertian>(color(.1, .9, .1));
    auto material3 = make_shared<lambertian>(color(.1, .1, .9));

    scene.add(std::make_shared<sphere>(point3(-1, 0, -1), .5, material1));
    scene.add(std::make_shared<sphere>(point3(0, 1, -1), .5, material2));
    scene.add(std::make_shared<sphere>(point3(1, 2, -1), .5, material3));
    sphere s = sphere(point3(0, 0, -1), .5, material1);
    // std::cout << s << std::endl;

    scene = object_list(make_shared<bvh_node>(scene));

    std::cout << scene << std::endl;

    camera cam;
    cam.aspect_ratio = 9. / 16.;
    cam.image_width = 400;
    cam.samples_per_pixel = 100;
    cam.max_depth = 50;

    cam.vfov = 95;

    cam.lookfrom = point3(0, 0, 1);
    cam.lookat = point3(0, 0, -1);

    cam.defocus_angle = 1;
    cam.focus_dist = 10;

    cam.render(scene);
}
