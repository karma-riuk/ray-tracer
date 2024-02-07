#include "color.hpp"

#include "interval.hpp"

inline color linear_to_gamma(color c) {
    return color(std::sqrt(c.x()), std::sqrt(c.y()), std::sqrt(c.z()));
}

void write_color(std::ostream& out, color pixel_color) {
    static const interval intesity(0, 0.999);
    pixel_color = linear_to_gamma(pixel_color);
    out << static_cast<int>(256 * intesity.clamp(pixel_color.x())) << ' '
        << static_cast<int>(256 * intesity.clamp(pixel_color.y())) << ' '
        << static_cast<int>(256 * intesity.clamp(pixel_color.z())) << '\n';
}
