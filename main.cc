#include "camera.hpp"
#include "objects/object_list.hpp"
#include "objects/sphere.hpp"
#include "rtweekend.hpp"

int main() {
    object_list scene;

    scene.add(std::make_shared<sphere>());
    scene.add(std::make_shared<sphere>(point3(0, -100.5, -1), 100));

    camera cam;
    cam.aspect_ratio = 9. / 16.;
    cam.image_width = 400;
    cam.samples_per_pixel = 100;

    cam.render(scene);
}
