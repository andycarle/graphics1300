#include "Triangle.h"
#include "Internal.h"
#include <algorithm>
#include <utility>

Triangle::Triangle(Point corner1, Point corner2, Point corner3, Color color)
    : corner1_(corner1), corner2_(corner2), corner3_(corner3),
      color_(color), outlineColor_(Color::Transparent) {}

// Fills a triangle one row of pixels at a time.
static void fillTriangle(Tigr* screen, int x1, int y1, int x2, int y2, int x3, int y3,
                         TPixel pixel) {
    // Sort the corners from top to bottom.
    if (y1 > y2) { std::swap(x1, x2); std::swap(y1, y2); }
    if (y1 > y3) { std::swap(x1, x3); std::swap(y1, y3); }
    if (y2 > y3) { std::swap(x2, x3); std::swap(y2, y3); }

    for (int y = y1; y <= y3; y++) {
        // One edge always runs from the top corner to the bottom corner...
        double xa = (y3 == y1) ? x1 : x1 + double(x3 - x1) * (y - y1) / (y3 - y1);
        // ...the other is the top-to-middle edge, then the middle-to-bottom edge.
        double xb;
        if (y < y2) xb = x1 + double(x2 - x1) * (y - y1) / (y2 - y1);
        else        xb = (y3 == y2) ? x2 : x2 + double(x3 - x2) * (y - y2) / (y3 - y2);
        if (y3 == y1) { xa = std::min({x1, x2, x3}); xb = std::max({x1, x2, x3}); }

        int left  = (int)(std::min(xa, xb) + 0.5);
        int right = (int)(std::max(xa, xb) + 0.5);
        internal::fillArea(screen, left, y, right - left + 1, 1, pixel);
    }
}

void Triangle::draw(Window& window) const {
    Tigr* screen = LibraryAccess::screen(window);
    if (screen == nullptr) return;
    double cx = window.getCameraX(), cy = window.getCameraY();
    int x1 = internal::roundToPixel(corner1_.x - cx), y1 = internal::roundToPixel(corner1_.y - cy);
    int x2 = internal::roundToPixel(corner2_.x - cx), y2 = internal::roundToPixel(corner2_.y - cy);
    int x3 = internal::roundToPixel(corner3_.x - cx), y3 = internal::roundToPixel(corner3_.y - cy);

    if (color_.a > 0) {
        fillTriangle(screen, x1, y1, x2, y2, x3, y3, internal::toShapePixel(color_));
    }
    if (outlineColor_.a > 0) {
        TPixel edge = internal::toShapePixel(outlineColor_);
        tigrLine(screen, x1, y1, x2, y2, edge);
        tigrLine(screen, x2, y2, x3, y3, edge);
        tigrLine(screen, x3, y3, x1, y1, edge);
    }
}

Point Triangle::getCorner1() const { return corner1_; }
Point Triangle::getCorner2() const { return corner2_; }
Point Triangle::getCorner3() const { return corner3_; }

void Triangle::setCorners(Point corner1, Point corner2, Point corner3) {
    corner1_ = corner1;
    corner2_ = corner2;
    corner3_ = corner3;
}

void Triangle::move(double dx, double dy) {
    corner1_.x += dx; corner1_.y += dy;
    corner2_.x += dx; corner2_.y += dy;
    corner3_.x += dx; corner3_.y += dy;
}

Color Triangle::getColor() const { return color_; }
void Triangle::setColor(Color color) { color_ = color; }

Color Triangle::getOutlineColor() const { return outlineColor_; }
void Triangle::setOutlineColor(Color color) { outlineColor_ = color; }
