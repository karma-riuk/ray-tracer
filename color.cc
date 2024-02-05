#include "color.hpp"

#include "interval.hpp"

void write_color(std::ostream& out, color pixel_color) {
    static const interval intesity(0, 0.999);
    out << static_cast<int>(256 * intesity.clamp(pixel_color.x())) << ' '
        << static_cast<int>(256 * intesity.clamp(pixel_color.y())) << ' '
        << static_cast<int>(256 * intesity.clamp(pixel_color.z())) << '\n';
}
