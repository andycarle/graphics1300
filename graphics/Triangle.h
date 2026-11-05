// Triangle.h - a triangle with three corners, with an optional outline.
//
//     Triangle roof(Point(320, 420), Point(420, 260), Point(520, 420), Color::Green);
//     roof.draw(window);
//
// For an outline with no fill, set the color to Color::Transparent.
#pragma once

#include "Window.h"
#include "Color.h"
#include "Point.h"

class Triangle {
public:
    // The corners can be given in any order.
    Triangle(Point corner1, Point corner2, Point corner3, Color color);

    // Draws the triangle on the window.
    void draw(Window& window) const;

    Point getCorner1() const;
    Point getCorner2() const;
    Point getCorner3() const;
    void setCorners(Point corner1, Point corner2, Point corner3);
    void move(double dx, double dy);   // moves all three corners by (dx, dy)

    Color getColor() const;            // the fill color
    void setColor(Color color);

    Color getOutlineColor() const;     // Color::Transparent = no outline (the default)
    void setOutlineColor(Color color);

private:
    Point corner1_, corner2_, corner3_;
    Color color_;
    Color outlineColor_;
};
