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

    vec3 earth_center = vec3(0, -1000, 0);
    double earth_radius = 1000;
    auto material_ground = make_shared<lambertian>(color(.5, .5, .5));
    scene.add(
        std::make_shared<sphere>(earth_center, earth_radius, material_ground)
    );
    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            double choose_mat = random_double();
            double radius = random_double(1, 5);
            point3 center(
                10 * a + 10 * random_double(),
                radius,
                10 * b + 10 * random_double()
            );


            vec3 E2C = center - earth_center;
            center = earth_center + (earth_radius * unit_vector(E2C))
                   + radius * unit_vector(E2C);

            shared_ptr<material> sphere_material;
            if (choose_mat < 0.8) {
                // Diffuse
                color albedo = color::random() * color::random();
                sphere_material = make_shared<lambertian>(albedo);
                scene.add(
                    std::make_shared<sphere>(center, radius, sphere_material)
                );
            } else if (choose_mat < 0.95) {
                // Metal
                color albedo = color::random(.5, 1);
                double fuzz = random_double(0, .5);
                sphere_material = make_shared<metal>(albedo, fuzz);
                scene.add(
                    std::make_shared<sphere>(center, radius, sphere_material)
                );
            } else {
                // Glass
                sphere_material = make_shared<dielectric>(1.5);
                scene.add(
                    std::make_shared<sphere>(center, radius, sphere_material)
                );
            }
        }
    }

    auto material1 = make_shared<dielectric>(1.5);
    auto material2 = make_shared<lambertian>(color(.4, .2, .1));
    auto material3 = make_shared<metal>(color(.7, .6, .9), 0.0);

    scene.add(std::make_shared<sphere>(point3(0, 1, 0), 1, material1));
    scene.add(std::make_shared<sphere>(point3(-4, 1, 0), 1, material2));
    scene.add(std::make_shared<sphere>(point3(4, 1, 0), 1, material3));

    scene = object_list(make_shared<bvh_node>(scene));

    camera cam;
    cam.aspect_ratio = 9. / 16.;
    cam.image_width = 800;
    cam.samples_per_pixel = 50;
    cam.max_depth = 8;

    cam.vfov = 20;

    cam.lookfrom = point3(130, 50, 80);
    cam.lookat = point3(0, 0, 0);

    cam.defocus_angle = 0.6;
    cam.focus_dist = 150;

    cam.render(scene);
}
