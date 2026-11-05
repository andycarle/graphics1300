// Player.h - the slime. Moves with WASD or the arrow keys; the Game
// makes it fire automatically.
#pragma once

#include "graphics/Window.h"
#include "graphics/Circle.h"
#include "graphics/Image.h"
#include "graphics/Point.h"
#include "graphics/Sprite.h"
#include "Upgrade.h"

class Player {
public:
    explicit Player(const Image& image);

    // Puts the player back in the middle with full health and starting stats.
    void reset();

    void update(Window& window, double dt);
    void draw(Window& window) const;

    // Loses one health, unless the player was hit very recently.
    // Returns true if the hit counted.
    bool takeDamage();

    void applyUpgrade(Upgrade upgrade);

    // Firing: the Game asks if we're ready, fires, then restarts the timer.
    bool isReadyToFire() const;
    void restartFireTimer();

    Point getPosition() const;    // the center of the slime
    double getRadius() const;     // for collisions
    int getHealth() const;
    int getMaxHealth() const;
    bool isDead() const;

    int getBulletCount() const;
    int getPierce() const;
    int getDamage() const;
    double getMagnetRange() const;

private:
    Point position_;
    int health_, maxHealth_;
    double invulnerableTimer_;   // seconds of safety left after being hit
    double fireTimer_;           // seconds until the next shot
    double wobble_;              // drives the squash-and-stretch animation

    // Stats that upgrades improve.
    double fireInterval_;
    double moveSpeed_;
    double magnetRange_;
    int bulletCount_;
    int pierce_;
    int damage_;

    Circle shadow_;
    Sprite sprite_;
};
