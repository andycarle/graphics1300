#include "Coin.h"
#include "MathHelpers.h"
#include "Settings.h"
#include <cmath>

const int COIN_SIZE = 32;   // on screen: the 16x16 frames drawn twice as big

Coin::Coin(const Image& sheet, Point start)
    : position_(start), age_(randomDouble(0, 1)), attracted_(false),
      sprite_(sheet, start, COIN_SIZE, COIN_SIZE) {
    // Pop out in a random direction.
    double angle = randomDouble(0, 2 * PI);
    double speed = randomDouble(60, 180);
    velocityX_ = std::cos(angle) * speed;
    velocityY_ = std::sin(angle) * speed;
}

void Coin::update(double dt, Point playerPosition, double magnetRange) {
    age_ += dt;

    double dist = position_.distanceTo(playerPosition);
    if (dist < magnetRange) attracted_ = true;   // once pulled in, it never lets go

    if (attracted_ && dist > 0) {
        // Accelerate toward the player.
        velocityX_ += (playerPosition.x - position_.x) / dist * 2200 * dt;
        velocityY_ += (playerPosition.y - position_.y) / dist * 2200 * dt;
    } else {
        // Slide to a stop.
        double drag = 1.0 - 4.0 * dt;
        if (drag < 0) drag = 0;
        velocityX_ *= drag;
        velocityY_ *= drag;
    }
    position_.x += velocityX_ * dt;
    position_.y += velocityY_ * dt;

    // Spin through the 4 frames of the sprite sheet.
    int frame = (int)(age_ * 10) % 4;
    sprite_.setFrame(frame * 16, 0, 16, 16);
    sprite_.setPosition(Point(position_.x - COIN_SIZE / 2.0, position_.y - COIN_SIZE / 2.0));
}

void Coin::draw(Window& window) const {
    sprite_.draw(window);
}

Point Coin::getPosition() const { return position_; }
