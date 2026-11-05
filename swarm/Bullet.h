// Bullet.h - a glowing shot fired by the player.
#pragma once

#include "graphics/Window.h"
#include "graphics/Circle.h"
#include "graphics/Point.h"
#include <vector>

class Bullet {
public:
    // Flies from `start` in the direction `angle` (in radians).
    // pierce = how many extra enemies it can pass through.
    Bullet(Point start, double angle, double speed, int damage, int pierce);

    void update(double dt);
    void draw(Window& window) const;

    Point getPosition() const;
    double getRadius() const;
    double getVelocityX() const;
    double getVelocityY() const;
    int getDamage() const;

    // True if this bullet already hit the enemy with this id.
    // (A piercing bullet must not hit the same enemy on every frame.)
    bool hasHit(int enemyId) const;

    // Remembers the hit and uses up one pierce.
    void recordHit(int enemyId);

    // True once the bullet has used up its hits or left the screen.
    bool isFinished() const;

private:
    Point position_;
    double velocityX_, velocityY_;
    int damage_;
    int hitsLeft_;
    std::vector<int> enemiesHit_;

    Circle glow_;
    Circle core_;
    std::vector<Circle> trail_;
};
