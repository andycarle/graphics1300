#include "Player.h"
#include "Settings.h"
#include "graphics/Color.h"
#include <cmath>

const double SLIME_SIZE = 48;   // on screen: the 16x16 image drawn 3x as big

Player::Player(const Image& image)
    : shadow_(Point(), 15, Color(0, 0, 0, 80)),
      sprite_(image, Point(), SLIME_SIZE, SLIME_SIZE) {
    reset();
}

void Player::reset() {
    position_ = Point(SCREEN_WIDTH / 2.0, SCREEN_HEIGHT / 2.0);
    maxHealth_ = 5;
    health_ = maxHealth_;
    invulnerableTimer_ = 0;
    fireTimer_ = 0;
    wobble_ = 0;

    fireInterval_ = 0.45;
    moveSpeed_ = 230;
    magnetRange_ = 90;
    bulletCount_ = 1;
    pierce_ = 0;
    damage_ = 1;
}

void Player::update(Window& window, double dt) {
    // Which way do the keys say to go?
    double dx = 0, dy = 0;
    if (window.isKeyDown(Key::Left)  || window.isKeyDown('A')) dx -= 1;
    if (window.isKeyDown(Key::Right) || window.isKeyDown('D')) dx += 1;
    if (window.isKeyDown(Key::Up)    || window.isKeyDown('W')) dy -= 1;
    if (window.isKeyDown(Key::Down)  || window.isKeyDown('S')) dy += 1;

    bool moving = (dx != 0 || dy != 0);
    if (moving) {
        // Divide by the length so moving diagonally isn't faster.
        double length = std::sqrt(dx * dx + dy * dy);
        position_.x += dx / length * moveSpeed_ * dt;
        position_.y += dy / length * moveSpeed_ * dt;
    }

    // Stay inside the arena.
    const double margin = 24;
    if (position_.x < margin) position_.x = margin;
    if (position_.x > SCREEN_WIDTH - margin) position_.x = SCREEN_WIDTH - margin;
    if (position_.y < margin) position_.y = margin;
    if (position_.y > SCREEN_HEIGHT - margin) position_.y = SCREEN_HEIGHT - margin;

    fireTimer_ -= dt;
    invulnerableTimer_ -= dt;

    // Squash and stretch: wobble quickly while moving, gently while still.
    wobble_ += dt * (moving ? 14 : 4);
    double squash = std::sin(wobble_) * (moving ? 0.12 : 0.05);
    double width = SLIME_SIZE * (1 + squash);
    double height = SLIME_SIZE * (1 - squash);
    // Keep the bottom of the slime on the ground while it squashes.
    double bottom = position_.y + SLIME_SIZE / 2;
    sprite_.setSize(width, height);
    sprite_.setPosition(Point(position_.x - width / 2, bottom - height));
    shadow_.setPosition(Point(position_.x, bottom - 4));
}

void Player::draw(Window& window) const {
    shadow_.draw(window);
    // Blink while invulnerable.
    bool blinkOff = invulnerableTimer_ > 0 && (int)(invulnerableTimer_ * 20) % 2 == 0;
    if (!blinkOff) sprite_.draw(window);
}

bool Player::takeDamage() {
    if (invulnerableTimer_ > 0 || health_ <= 0) return false;
    health_--;
    invulnerableTimer_ = 1.0;
    return true;
}

void Player::applyUpgrade(Upgrade upgrade) {
    switch (upgrade) {
        case Upgrade::RapidFire:   fireInterval_ *= 0.8; break;
        case Upgrade::Multishot:   bulletCount_++; break;
        case Upgrade::Piercing:    pierce_++; break;
        case Upgrade::HeavyRounds: damage_++; break;
        case Upgrade::SwiftFeet:   moveSpeed_ *= 1.15; break;
        case Upgrade::Magnet:      magnetRange_ *= 1.6; break;
        case Upgrade::PatchUp:     maxHealth_++; health_ = maxHealth_; break;
    }
}

bool Player::isReadyToFire() const { return fireTimer_ <= 0; }
void Player::restartFireTimer() { fireTimer_ = fireInterval_; }

Point Player::getPosition() const { return position_; }
double Player::getRadius() const { return 18; }
int Player::getHealth() const { return health_; }
int Player::getMaxHealth() const { return maxHealth_; }
bool Player::isDead() const { return health_ <= 0; }

int Player::getBulletCount() const { return bulletCount_; }
int Player::getPierce() const { return pierce_; }
int Player::getDamage() const { return damage_; }
double Player::getMagnetRange() const { return magnetRange_; }
