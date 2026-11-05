// UpgradeCard.h - one of the three cards shown when the player levels up.
// Cards slide up into place, lift when the mouse is over them, and are
// chosen with a click or the number keys 1, 2, 3.
#pragma once

#include "graphics/Window.h"
#include "graphics/Circle.h"
#include "graphics/Rectangle.h"
#include "graphics/Text.h"
#include "Upgrade.h"

class UpgradeCard {
public:
    static const int WIDTH = 240;
    static const int HEIGHT = 290;

    // (x, targetY) is where the card's top-left corner ends up.
    // keyNumber is the number key that picks this card.
    // delay is how many seconds to wait before sliding in.
    UpgradeCard(Upgrade upgrade, double x, double targetY, int keyNumber, double delay);

    void update(Window& window, double dt);
    void draw(Window& window) const;

    // True on the frame the player picks this card.
    bool wasChosen(const Window& window) const;

    Upgrade getUpgrade() const;

private:
    void layout();   // moves every shape to match the card's position

    Upgrade upgrade_;
    double x_, y_, targetY_;
    double delay_;
    double lift_;    // 0 = resting, 1 = fully lifted by the mouse
    int keyNumber_;
    bool hovered_;

    Rectangle shadow_;
    Rectangle background_;
    Rectangle band_;
    Circle iconOuter_;
    Circle iconInner_;
    Text iconLetter_;
    Text title_;
    Text description_;
    Text keyHint_;
};
