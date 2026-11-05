#include "Point.h"
#include <cmath>

Point::Point() : x(0), y(0) {}

Point::Point(double xPosition, double yPosition) : x(xPosition), y(yPosition) {}

double Point::distanceTo(Point other) const {
    double dx = other.x - x;
    double dy = other.y - y;
    return std::sqrt(dx * dx + dy * dy);
}
