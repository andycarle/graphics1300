#include "Circle.h"
#include "Internal.h"

Circle::Circle(Point center, double radius, Color color)
    : center_(center), radius_(radius),
      color_(color), outlineColor_(Color::Transparent) {}

void Circle::draw(Window& window) const {
    Tigr* screen = LibraryAccess::screen(window);
    if (screen == nullptr || radius_ < 0) return;
    int x = internal::roundToPixel(center_.x - window.getCameraX());
    int y = internal::roundToPixel(center_.y - window.getCameraY());
    int r = internal::roundToPixel(radius_);

    // TIGR's fill covers the inside and its outline covers the edge,
    // without overlapping, so see-through colors still look right.
    // With no outline, the edge is drawn in the fill color.
    Color edge = (outlineColor_.a <= 0) ? color_ : outlineColor_;
    tigrFillCircle(screen, x, y, r, internal::toShapePixel(color_));
    tigrCircle(screen, x, y, r, internal::toShapePixel(edge));
}

Point Circle::getPosition() const { return center_; }
void Circle::setPosition(Point center) { center_ = center; }
double Circle::getX() const { return center_.x; }
double Circle::getY() const { return center_.y; }
void Circle::setX(double x) { center_.x = x; }
void Circle::setY(double y) { center_.y = y; }
void Circle::move(double dx, double dy) { center_.x += dx; center_.y += dy; }

double Circle::getRadius() const { return radius_; }
void Circle::setRadius(double radius) { radius_ = radius; }

Color Circle::getColor() const { return color_; }
void Circle::setColor(Color color) { color_ = color; }

Color Circle::getOutlineColor() const { return outlineColor_; }
void Circle::setOutlineColor(Color color) { outlineColor_ = color; }

bool Circle::contains(Point point) const {
    return center_.distanceTo(point) <= radius_;
}
