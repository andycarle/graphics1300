// Rectangle.h - a rectangle, with an optional outline.
//
//     Rectangle box(Point(60, 60), 200, 120, Color::Blue);
//     box.setOutlineColor(Color::White);
//     box.draw(window);
//
// For an outline with no fill, set the color to Color::Transparent.
#pragma once

#include "Window.h"
#include "Color.h"
#include "Point.h"

class Rectangle {
public:
    Rectangle(Point topLeft, double width, double height, Color color);

    // Draws the rectangle on the window.
    void draw(Window& window) const;

    Point getPosition() const;         // the top-left corner
    void setPosition(Point topLeft);
    double getX() const;
    double getY() const;
    void setX(double x);
    void setY(double y);
    void move(double dx, double dy);   // moves by (dx, dy)

    double getWidth() const;
    double getHeight() const;
    void setSize(double width, double height);

    Color getColor() const;            // the fill color
    void setColor(Color color);

    Color getOutlineColor() const;     // Color::Transparent = no outline (the default)
    void setOutlineColor(Color color);

    // True if the point is inside the rectangle.
    // Handy for mouse clicks: box.contains(window.getMousePosition())
    bool contains(Point point) const;

private:
    Point position_;
    double width_, height_;
    Color color_;
    Color outlineColor_;
};
