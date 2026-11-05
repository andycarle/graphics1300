// Line.h - a straight line between two points.
//
//     Line laser(Point(40, 440), Point(600, 300), Color::Yellow);
//     laser.draw(window);
#pragma once

#include "Window.h"
#include "Color.h"
#include "Point.h"

class Line {
public:
    Line(Point start, Point end, Color color);

    // Draws the line on the window.
    void draw(Window& window) const;

    Point getStart() const;
    Point getEnd() const;
    void setStart(Point start);
    void setEnd(Point end);
    void move(double dx, double dy);   // moves both ends by (dx, dy)

    Color getColor() const;
    void setColor(Color color);

private:
    Point start_, end_;
    Color color_;
};
