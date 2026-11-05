// Circle.h - a circle, with an optional outline.
//
//     Circle ball(Point(320, 240), 50, Color::Red);
//     ball.draw(window);
//
// For an outline with no fill, set the color to Color::Transparent.
#pragma once

#include "Window.h"
#include "Color.h"
#include "Point.h"

class Circle {
public:
    Circle(Point center, double radius, Color color);

    // Draws the circle on the window.
    void draw(Window& window) const;

    Point getPosition() const;         // the center
    void setPosition(Point center);
    double getX() const;
    double getY() const;
    void setX(double x);
    void setY(double y);
    void move(double dx, double dy);   // moves by (dx, dy)

    double getRadius() const;
    void setRadius(double radius);

    Color getColor() const;            // the fill color
    void setColor(Color color);

    Color getOutlineColor() const;     // Color::Transparent = no outline (the default)
    void setOutlineColor(Color color);

    // True if the point is inside the circle.
    // Handy for mouse clicks: ball.contains(window.getMousePosition())
    bool contains(Point point) const;

private:
    Point center_;
    double radius_;
    Color color_;
    Color outlineColor_;
};
