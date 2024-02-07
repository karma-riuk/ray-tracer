#include "camera.hpp"
#include "materials/lambertian.hpp"
#include "materials/metal.hpp"
#include "objects/object_list.hpp"
#include "objects/sphere.hpp"

int main() {
    object_list scene;

    auto material_ground = make_shared<lambertian>(color(.8, .8, 0));
    auto material_center = make_shared<lambertian>(color(.7, .3, .3));
    auto material_left = make_shared<metal>(color(.8, .8, .8), .2);
    auto material_right = make_shared<metal>(color(.3, .3, .7), .9);

    scene.add(std::make_shared<sphere>(point3(0, 0, -1), .5, material_center));
    scene.add(
        std::make_shared<sphere>(point3(0, -100.5, -1), 100, material_ground)
    );
    scene.add(std::make_shared<sphere>(point3(-1, 0, -1), .5, material_left));
    scene.add(std::make_shared<sphere>(point3(1, 0, -1), .5, material_right));

    camera cam;
    cam.aspect_ratio = 9. / 16.;
    cam.image_width = 800;
    cam.samples_per_pixel = 100;
    cam.max_depth = 50;

    cam.render(scene);
}
