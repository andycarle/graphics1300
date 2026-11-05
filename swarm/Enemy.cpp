#include "Enemy.h"
#include "MathHelpers.h"
#include <cmath>

const double SPAWN_TIME = 0.3;   // seconds an enemy takes to grow to full size

Enemy::Enemy(EnemyType type, Point start, int id)
    : type_(type), id_(id), position_(start), pushX_(0), pushY_(0),
      age_(0), flashTimer_(0), facing_(0),
      dashPhase_(0), dashTimer_(randomDouble(1.0, 2.0)), dashX_(0), dashY_(0),
      triangle_(Point(), Point(), Point(), Color::Red),
      circle_(Point(), 0, Color::Cyan),
      warningRing_(Point(), 0, Color::Transparent),
      square_(Point(), 0, 0, Color::Purple),
      innerSquare_(Point(), 0, 0, Color(60, 20, 110)),
      healthBack_(Point(), 0, 0, Color(0, 0, 0, 160)),
      healthFill_(Point(), 0, 0, Color(120, 255, 140)) {
    switch (type_) {
        case EnemyType::Drone:
            radius_ = 14; speed_ = 105; maxHealth_ = 2;
            color_ = Color(255, 80, 100);
            break;
        case EnemyType::Darter:
            radius_ = 12; speed_ = 70; maxHealth_ = 1;
            color_ = Color(0, 220, 230);
            break;
        case EnemyType::Brute:
            radius_ = 24; speed_ = 50; maxHealth_ = 12;
            color_ = Color(170, 100, 255);
            break;
    }
    health_ = maxHealth_;
    triangle_.setOutlineColor(Color(255, 255, 255, 110));
    square_.setOutlineColor(Color(255, 255, 255, 110));
    updateShapes();
}

void Enemy::update(double dt, Point playerPosition) {
    age_ += dt;
    flashTimer_ -= dt;

    // Direction to the player, as a length-1 arrow (dirX, dirY).
    double dist = position_.distanceTo(playerPosition);
    double dirX = 0, dirY = 0;
    if (dist > 0) {
        dirX = (playerPosition.x - position_.x) / dist;
        dirY = (playerPosition.y - position_.y) / dist;
    }
    facing_ = std::atan2(dirY, dirX);

    if (type_ == EnemyType::Darter) {
        dashTimer_ -= dt;
        if (dashPhase_ == 0) {                  // creep closer
            position_.x += dirX * speed_ * dt;
            position_.y += dirY * speed_ * dt;
            if (dashTimer_ <= 0) { dashPhase_ = 1; dashTimer_ = 0.5; }
        } else if (dashPhase_ == 1) {           // hold still and flash a warning
            dashX_ = dirX;
            dashY_ = dirY;
            if (dashTimer_ <= 0) { dashPhase_ = 2; dashTimer_ = 0.35; }
        } else {                                // DASH!
            position_.x += dashX_ * 560 * dt;
            position_.y += dashY_ * 560 * dt;
            if (dashTimer_ <= 0) { dashPhase_ = 0; dashTimer_ = randomDouble(1.2, 2.2); }
        }
    } else {
        position_.x += dirX * speed_ * dt;
        position_.y += dirY * speed_ * dt;
    }

    // Knockback slides the enemy, then fades away.
    position_.x += pushX_ * dt;
    position_.y += pushY_ * dt;
    double drag = 1.0 - 8.0 * dt;
    if (drag < 0) drag = 0;
    pushX_ *= drag;
    pushY_ *= drag;

    updateShapes();
}

// Moves and colors this enemy's shapes to match its current state.
void Enemy::updateShapes() {
    double grow = age_ / SPAWN_TIME;
    if (grow > 1) grow = 1;
    double r = radius_ * grow;
    Color body = flashTimer_ > 0 ? Color::White : color_;
    double x = position_.x, y = position_.y;

    if (type_ == EnemyType::Drone) {
        // A triangle whose tip points at the player.
        triangle_.setCorners(pointAtAngle(position_, facing_, r * 1.4),
                             pointAtAngle(position_, facing_ + 2.5, r),
                             pointAtAngle(position_, facing_ - 2.5, r));
        triangle_.setColor(body);
    } else if (type_ == EnemyType::Darter) {
        if (dashPhase_ == 1) {
            // Warning: blink white, with a ring that closes in.
            bool blink = (int)(dashTimer_ * 16) % 2 == 0;
            if (blink) body = Color::White;
            warningRing_.setPosition(position_);
            warningRing_.setRadius(r + 4 + 30 * (dashTimer_ / 0.5));
            warningRing_.setOutlineColor(color_.withAlpha(200));
        } else {
            warningRing_.setOutlineColor(Color::Transparent);
        }
        circle_.setPosition(position_);
        circle_.setRadius(r);
        circle_.setColor(body);
        circle_.setOutlineColor(dashPhase_ == 2 ? Color::White : Color(255, 255, 255, 110));
    } else {
        square_.setPosition(Point(x - r, y - r));
        square_.setSize(2 * r, 2 * r);
        square_.setColor(body);
        innerSquare_.setPosition(Point(x - r / 2, y - r / 2));
        innerSquare_.setSize(r, r);
        // Health bar above the brute.
        double fraction = (double)health_ / maxHealth_;
        Point barCorner(x - radius_, y - radius_ - 12);
        healthBack_.setPosition(barCorner);
        healthBack_.setSize(2 * radius_, 5);
        healthFill_.setPosition(barCorner);
        healthFill_.setSize(2 * radius_ * fraction, 5);
    }
}

void Enemy::draw(Window& window) const {
    switch (type_) {
        case EnemyType::Drone:
            triangle_.draw(window);
            break;
        case EnemyType::Darter:
            warningRing_.draw(window);
            circle_.draw(window);
            break;
        case EnemyType::Brute:
            square_.draw(window);
            innerSquare_.draw(window);
            if (health_ < maxHealth_) {
                healthBack_.draw(window);
                healthFill_.draw(window);
            }
            break;
    }
}

void Enemy::hit(int damage, double pushX, double pushY) {
    health_ -= damage;
    flashTimer_ = 0.08;
    // Brutes are heavy: knockback barely moves them.
    double weight = (type_ == EnemyType::Brute) ? 4.0 : 1.0;
    push(pushX / weight, pushY / weight);
    updateShapes();
}

void Enemy::push(double pushX, double pushY) {
    pushX_ += pushX;
    pushY_ += pushY;
}

void Enemy::nudge(double dx, double dy) {
    position_.x += dx;
    position_.y += dy;
    updateShapes();
}

bool Enemy::isDead() const { return health_ <= 0; }
Point Enemy::getPosition() const { return position_; }
double Enemy::getRadius() const { return radius_; }
int Enemy::getId() const { return id_; }
EnemyType Enemy::getType() const { return type_; }
Color Enemy::getColor() const { return color_; }

int Enemy::getCoinValue() const {
    return type_ == EnemyType::Brute ? 4 : 1;
}
