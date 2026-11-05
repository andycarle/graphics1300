// Text.h - words drawn in the built-in pixel font.
//
//     Text score("Score: 0", Point(16, 16), Color::Yellow, 3);
//     score.setText("Score: " + std::to_string(points));
//     score.draw(window);
#pragma once

#include "Window.h"
#include "Color.h"
#include "Point.h"
#include <string>

class Text {
public:
    // scale 1 is the normal font size; scale 2 is twice as big,
    // 3 is three times, and so on.
    Text(const std::string& text, Point topLeft, Color color, int scale = 1);

    // Draws the text on the window.
    void draw(Window& window) const;

    std::string getText() const;
    void setText(const std::string& text);

    Point getPosition() const;         // the top-left corner
    void setPosition(Point topLeft);
    double getX() const;
    double getY() const;
    void setX(double x);
    void setY(double y);
    void move(double dx, double dy);   // moves by (dx, dy)

    int getScale() const;
    void setScale(int scale);

    // Size of the text on screen, in pixels. Useful for centering.
    double getWidth() const;
    double getHeight() const;

    Color getColor() const;
    void setColor(Color color);

private:
    std::string text_;
    Point position_;
    Color color_;
    int scale_;
};
