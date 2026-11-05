#include "Bullet.h"
#include "Settings.h"
#include "graphics/Color.h"
#include <cmath>

Bullet::Bullet(Point start, double angle, double speed, int damage, int pierce)
    : position_(start),
      velocityX_(std::cos(angle) * speed), velocityY_(std::sin(angle) * speed),
      damage_(damage), hitsLeft_(1 + pierce),
      glow_(start, 7, Color(255, 220, 80, 90)),
      core_(start, 3, Color::White) {
    // A short tail of fading dots behind the bullet.
    for (int i = 0; i < 4; i++) {
        trail_.push_back(Circle(start, 4 - i * 0.7, Color(255, 200, 60, 150 - i * 35)));
    }
}

void Bullet::update(double dt) {
    position_.x += velocityX_ * dt;
    position_.y += velocityY_ * dt;

    glow_.setPosition(position_);
    core_.setPosition(position_);
    for (size_t i = 0; i < trail_.size(); i++) {
        double back = 0.01 * (i + 1);   // how many seconds behind the bullet
        trail_[i].setPosition(Point(position_.x - velocityX_ * back,
                                    position_.y - velocityY_ * back));
    }
}

void Bullet::draw(Window& window) const {
    for (const Circle& dot : trail_) {
        dot.draw(window);
    }
    glow_.draw(window);
    core_.draw(window);
}

Point Bullet::getPosition() const { return position_; }
double Bullet::getRadius() const { return 5; }
double Bullet::getVelocityX() const { return velocityX_; }
double Bullet::getVelocityY() const { return velocityY_; }
int Bullet::getDamage() const { return damage_; }

bool Bullet::hasHit(int enemyId) const {
    for (int id : enemiesHit_) {
        if (id == enemyId) return true;
    }
    return false;
}

void Bullet::recordHit(int enemyId) {
    enemiesHit_.push_back(enemyId);
    hitsLeft_--;
}

bool Bullet::isFinished() const {
    const int margin = 30;
    bool offScreen = position_.x < -margin || position_.x > SCREEN_WIDTH + margin ||
                     position_.y < -margin || position_.y > SCREEN_HEIGHT + margin;
    return hitsLeft_ <= 0 || offScreen;
}
