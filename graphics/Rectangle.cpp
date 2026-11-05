#include "Rectangle.h"
#include "Internal.h"

Rectangle::Rectangle(Point topLeft, double width, double height, Color color)
    : position_(topLeft), width_(width), height_(height),
      color_(color), outlineColor_(Color::Transparent) {}

void Rectangle::draw(Window& window) const {
    Tigr* screen = LibraryAccess::screen(window);
    if (screen == nullptr) return;
    int x = internal::roundToPixel(position_.x - window.getCameraX());
    int y = internal::roundToPixel(position_.y - window.getCameraY());
    int w = internal::roundToPixel(width_);
    int h = internal::roundToPixel(height_);
    if (w <= 0 || h <= 0) return;

    if (outlineColor_.a <= 0) {
        internal::fillArea(screen, x, y, w, h, internal::toShapePixel(color_));
    } else {
        // TIGR's fill covers the inside and its outline covers the edge,
        // without overlapping, so see-through colors still look right.
        tigrFillRect(screen, x, y, w, h, internal::toShapePixel(color_));
        tigrRect(screen, x, y, w, h, internal::toShapePixel(outlineColor_));
    }
}

Point Rectangle::getPosition() const { return position_; }
void Rectangle::setPosition(Point topLeft) { position_ = topLeft; }
double Rectangle::getX() const { return position_.x; }
double Rectangle::getY() const { return position_.y; }
void Rectangle::setX(double x) { position_.x = x; }
void Rectangle::setY(double y) { position_.y = y; }
void Rectangle::move(double dx, double dy) { position_.x += dx; position_.y += dy; }

double Rectangle::getWidth() const  { return width_; }
double Rectangle::getHeight() const { return height_; }
void Rectangle::setSize(double width, double height) { width_ = width; height_ = height; }

Color Rectangle::getColor() const { return color_; }
void Rectangle::setColor(Color color) { color_ = color; }

Color Rectangle::getOutlineColor() const { return outlineColor_; }
void Rectangle::setOutlineColor(Color color) { outlineColor_ = color; }

bool Rectangle::contains(Point point) const {
    return point.x >= position_.x && point.x < position_.x + width_ &&
           point.y >= position_.y && point.y < position_.y + height_;
}
