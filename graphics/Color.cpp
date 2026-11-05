#include "Color.h"

Color::Color() : r(0), g(0), b(0), a(255) {}

Color::Color(int red, int green, int blue, int alpha)
    : r(red), g(green), b(blue), a(alpha) {}

Color Color::withAlpha(int alpha) const {
    return Color(r, g, b, alpha);
}
