#include "FloatingText.h"
#include "graphics/Point.h"

FloatingText::FloatingText(const std::string& message, double centerX, double y,
                           Color color, int scale, double lifetime)
    : text_(message, Point(0, y), color, scale),
      shadow_(message, Point(0, y), Color(0, 0, 0, 160), scale),
      color_(color), age_(0), lifetime_(lifetime) {
    double left = centerX - text_.getWidth() / 2;
    text_.setX(left);
    shadow_.setPosition(Point(left + scale, y + scale));
}

void FloatingText::update(double dt) {
    age_ += dt;
    text_.move(0, -30 * dt);
    shadow_.move(0, -30 * dt);

    // Fully visible for the first 60% of its life, then fade out.
    double fade = 1.0;
    double fadeStart = lifetime_ * 0.6;
    if (age_ > fadeStart) fade = 1.0 - (age_ - fadeStart) / (lifetime_ - fadeStart);
    if (fade < 0) fade = 0;
    text_.setColor(color_.withAlpha((int)(255 * fade)));
    shadow_.setColor(Color(0, 0, 0, (int)(160 * fade)));
}

void FloatingText::draw(Window& window) const {
    shadow_.draw(window);
    text_.draw(window);
}

bool FloatingText::isAlive() const {
    return age_ < lifetime_;
}
