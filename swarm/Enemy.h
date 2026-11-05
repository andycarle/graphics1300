// Enemy.h - one member of the swarm. There are three kinds:
//   Drone   a red triangle that steadily chases the player
//   Darter  a cyan circle that creeps closer, flashes, then dashes
//   Brute   a big purple square that is slow but very tough
#pragma once

#include "graphics/Window.h"
#include "graphics/Circle.h"
#include "graphics/Color.h"
#include "graphics/Point.h"
#include "graphics/Rectangle.h"
#include "graphics/Triangle.h"

enum class EnemyType { Drone, Darter, Brute };

class Enemy {
public:
    // id must be different for every enemy (bullets use it to remember hits).
    Enemy(EnemyType type, Point start, int id);

    void update(double dt, Point playerPosition);
    void draw(Window& window) const;

    // Takes damage, flashes white, and gets knocked back by (pushX, pushY).
    void hit(int damage, double pushX, double pushY);

    // Adds to the enemy's knockback speed without damaging it.
    void push(double pushX, double pushY);

    // Moves the enemy directly (used to keep enemies from overlapping).
    void nudge(double dx, double dy);

    bool isDead() const;
    Point getPosition() const;  // the center of the enemy
    double getRadius() const;
    int getId() const;
    EnemyType getType() const;
    Color getColor() const;
    int getCoinValue() const;   // how many coins it drops

private:
    void updateShapes();

    EnemyType type_;
    int id_;
    Point position_;
    double pushX_, pushY_;   // knockback speed, which fades quickly
    double radius_;
    double speed_;
    int health_, maxHealth_;
    Color color_;
    double age_;
    double flashTimer_;      // white flash after being hit
    double facing_;          // angle toward the player, in radians

    // Darters: 0 = creeping closer, 1 = warning flash, 2 = dashing
    int dashPhase_;
    double dashTimer_;
    double dashX_, dashY_;   // locked-in dash direction

    // Each kind of enemy uses some of these shapes.
    Triangle triangle_;
    Circle circle_;
    Circle warningRing_;
    Rectangle square_;
    Rectangle innerSquare_;
    Rectangle healthBack_;
    Rectangle healthFill_;
};
