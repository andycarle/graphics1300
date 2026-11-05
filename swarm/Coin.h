// Coin.h - a spinning coin dropped by a defeated enemy.
// It pops out, settles, and flies to the player once they get close enough.
#pragma once

#include "graphics/Window.h"
#include "graphics/Image.h"
#include "graphics/Point.h"
#include "graphics/Sprite.h"

class Coin {
public:
    // sheet is the coin sprite sheet: four 16x16 frames side by side.
    // The coin starts centered on `start`.
    Coin(const Image& sheet, Point start);

    // magnetRange: how close the player must be before the coin flies to them.
    void update(double dt, Point playerPosition, double magnetRange);
    void draw(Window& window) const;

    Point getPosition() const;   // the center of the coin

private:
    Point position_;
    double velocityX_, velocityY_;
    double age_;
    bool attracted_;
    Sprite sprite_;
};
