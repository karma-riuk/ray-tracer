#include <fstream>
#include <iostream>

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

    progress += 0.16; // for demonstration only
}

int main() {
    // Image
    int image_width = 256;
    int image_height = 256;
    std::string filename = "image.ppm";
    std::ofstream file(filename);

    // Render
    file << "P3\n" << image_width << " " << image_height << "\n255\n";

    std::clog << "Writing to " << filename << std::endl;
    for (int j = 0; j < image_height; ++j) {
        for (int i = 0; i < image_width; ++i) {
            auto r = double(i) / (image_width - 1);
            auto g = double(j) / (image_height - 1);
            auto b = 0.25;

            int ir = static_cast<int>(255.999 * r);
            int ig = static_cast<int>(255.999 * g);
            int ib = static_cast<int>(255.999 * b);

            double progress =
                (double) (i + j * image_width) / (image_width * image_height);
            progress_bar(progress);
            file << ir << ' ' << ig << ' ' << ib << '\n';
        }
    }
    std::cout << std::endl;
    file.close();
}
