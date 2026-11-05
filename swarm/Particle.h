// Particle.h - a little dot of light that flies outward, slows down,
// shrinks and fades away. Explosions are just lots of these.
#pragma once

#include "graphics/Window.h"
#include "graphics/Circle.h"
#include "graphics/Color.h"
#include "graphics/Point.h"

class Particle {
public:
    Particle(Point start, double velocityX, double velocityY,
             double radius, Color color, double lifetime);

    void update(double dt);
    void draw(Window& window) const;

    // False once the particle has completely faded away.
    bool isAlive() const;

private:
    Point position_;
    double velocityX_, velocityY_;
    double startRadius_;
    double age_, lifetime_;
    Color color_;
    Circle circle_;
};
