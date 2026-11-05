#include "Particle.h"

Particle::Particle(Point start, double velocityX, double velocityY,
                   double radius, Color color, double lifetime)
    : position_(start), velocityX_(velocityX), velocityY_(velocityY),
      startRadius_(radius), age_(0), lifetime_(lifetime),
      color_(color), circle_(start, radius, color) {}

void Particle::update(double dt) {
    age_ += dt;
    position_.x += velocityX_ * dt;
    position_.y += velocityY_ * dt;

    // Air resistance: lose some speed every frame.
    double drag = 1.0 - 3.0 * dt;
    if (drag < 0) drag = 0;
    velocityX_ *= drag;
    velocityY_ *= drag;

    // Shrink and fade as the particle gets older.
    double lifeLeft = 1.0 - age_ / lifetime_;
    if (lifeLeft < 0) lifeLeft = 0;
    circle_.setPosition(position_);
    circle_.setRadius(startRadius_ * lifeLeft);
    circle_.setColor(color_.withAlpha((int)(color_.a * lifeLeft)));
}

void Particle::draw(Window& window) const {
    if (isAlive()) circle_.draw(window);
}

bool Particle::isAlive() const {
    return age_ < lifetime_;
}
