#include "camera.hpp"
#include "materials/dielectric.hpp"
#include "materials/lambertian.hpp"
#include "materials/metal.hpp"
#include "objects/object_list.hpp"
#include "objects/sphere.hpp"

int main() {
    object_list scene;

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

    auto material1 = make_shared<dielectric>(1.5);
    scene.add(std::make_shared<sphere>(point3(0, 1, 0), 1, material1));
    auto material2 = make_shared<lambertian>(color(.4, .2, .1));
    scene.add(std::make_shared<sphere>(point3(-4, 1, 0), 1, material2));
    auto material3 = make_shared<metal>(color(.7, .3, .2), 0);
    scene.add(std::make_shared<sphere>(point3(4, 1, 0), 1, material3));


    camera cam;
    cam.aspect_ratio = 9. / 16.;
    cam.image_width = 1200;
    cam.samples_per_pixel = 500;
    cam.max_depth = 50;

    cam.vfov = 20;

    cam.lookfrom = point3(13, 2, 3);
    cam.lookat = point3(0, 0, 0);

    cam.defocus_angle = 1;
    cam.focus_dist = 10;

    cam.render(scene);
}
