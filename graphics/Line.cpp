#include "Line.h"
#include "Internal.h"

Line::Line(Point start, Point end, Color color)
    : start_(start), end_(end), color_(color) {}

void Line::draw(Window& window) const {
    Tigr* screen = LibraryAccess::screen(window);
    if (screen == nullptr) return;
    double cx = window.getCameraX(), cy = window.getCameraY();
    int x1 = internal::roundToPixel(start_.x - cx), y1 = internal::roundToPixel(start_.y - cy);
    int x2 = internal::roundToPixel(end_.x - cx),   y2 = internal::roundToPixel(end_.y - cy);
    TPixel pixel = internal::toShapePixel(color_);
    tigrLine(screen, x1, y1, x2, y2, pixel);
    // tigrLine leaves out the end pixel; draw it so the line reaches the end.
    if (x1 != x2 || y1 != y2) tigrPlot(screen, x2, y2, pixel);
}

Point Line::getStart() const { return start_; }
Point Line::getEnd() const { return end_; }
void Line::setStart(Point start) { start_ = start; }
void Line::setEnd(Point end) { end_ = end; }
void Line::move(double dx, double dy) {
    start_.x += dx; start_.y += dy;
    end_.x += dx;   end_.y += dy;
}

Color Line::getColor() const { return color_; }
void Line::setColor(Color color) { color_ = color; }
