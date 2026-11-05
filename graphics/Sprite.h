// Sprite.h - an Image placed on the screen.
//
// The Image is the picture; a Sprite is where (and how big) to draw it.
// Many Sprites can share one Image.
//
//     Image slimeImage("slime.png");          // load once, before the game loop
//     Sprite player(slimeImage, Point(100, 300), 64, 64);
//     player.draw(window);
//
// Sprite sheets: one image holding several animation frames side by side.
// setFrame picks which part of the image to show:
//     coin.setFrame(frame * 16, 0, 16, 16);   // 16x16 frame number `frame`
#pragma once

#include "Window.h"
#include "Image.h"
#include "Point.h"

class Sprite {
public:
    // Shows the whole image at its normal size.
    Sprite(const Image& image, Point topLeft);

    // Shows the whole image stretched to width x height.
    Sprite(const Image& image, Point topLeft, double width, double height);

    // Draws the sprite on the window.
    void draw(Window& window) const;

    Point getPosition() const;         // the top-left corner
    void setPosition(Point topLeft);
    double getX() const;
    double getY() const;
    void setX(double x);
    void setY(double y);
    void move(double dx, double dy);   // moves by (dx, dy)

    double getWidth() const;           // size on screen
    double getHeight() const;
    void setSize(double width, double height);

    // Shows only part of the image: the rectangle starting at
    // (sourceX, sourceY) with size sourceWidth x sourceHeight.
    void setFrame(int sourceX, int sourceY, int sourceWidth, int sourceHeight);

    // Switches to a different image, showing all of it. The size on screen stays the same.
    void setImage(const Image& image);

    // True if the point is inside the sprite's rectangle on screen.
    // Handy for mouse clicks: player.contains(window.getMousePosition())
    bool contains(Point point) const;

private:
    Image image_;
    Point position_;
    double width_, height_;
    int sourceX_, sourceY_, sourceWidth_, sourceHeight_;
};
