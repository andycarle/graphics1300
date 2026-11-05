// Point.h - a position in the window: an x and a y.
//
//     Point corner(60, 60);
//     corner.x += 10;
//     double d = corner.distanceTo(Point(100, 100));
#pragma once

class Point {
public:
    double x, y;

    // The point (0, 0), the top-left corner of the window.
    Point();

    Point(double x, double y);

    // The straight-line distance from this point to another one.
    double distanceTo(Point other) const;
};
