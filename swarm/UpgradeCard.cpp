#include "UpgradeCard.h"
#include "Settings.h"
#include "graphics/Color.h"
#include "graphics/Point.h"

UpgradeCard::UpgradeCard(Upgrade upgrade, double x, double targetY, int keyNumber, double delay)
    : upgrade_(upgrade), x_(x), y_(SCREEN_HEIGHT + 40), targetY_(targetY),
      delay_(delay), lift_(0), keyNumber_(keyNumber), hovered_(false),
      shadow_(Point(), WIDTH, HEIGHT, Color(0, 0, 0, 120)),
      background_(Point(), WIDTH, HEIGHT, Color(34, 30, 56)),
      band_(Point(), WIDTH, 10, getUpgradeColor(upgrade)),
      iconOuter_(Point(), 44, getUpgradeColor(upgrade).withAlpha(70)),
      iconInner_(Point(), 32, getUpgradeColor(upgrade)),
      iconLetter_(getUpgradeName(upgrade).substr(0, 1), Point(), Color(34, 30, 56), 4),
      title_(getUpgradeName(upgrade), Point(), Color::White, 2),
      description_(getUpgradeDescription(upgrade), Point(), Color::LightGray),
      keyHint_("Press " + std::to_string(keyNumber) + " or click", Point(), Color::Gray) {
    layout();
}

void UpgradeCard::update(Window& window, double dt) {
    delay_ -= dt;
    if (delay_ <= 0) {
        // Slide toward the target, covering a fraction of the remaining
        // distance each frame, so the card slows down as it arrives.
        double step = 12 * dt;
        if (step > 1) step = 1;
        y_ += (targetY_ - y_) * step;
    }

    // Hovering uses the card's resting spot, so lifting it doesn't
    // move the edge out from under the mouse.
    Point mouse = window.getMousePosition();
    hovered_ = mouse.x >= x_ && mouse.x < x_ + WIDTH && mouse.y >= y_ && mouse.y < y_ + HEIGHT;

    // Ease the lift toward 1 when hovered, back to 0 when not.
    double goal = hovered_ ? 1.0 : 0.0;
    double step = 15 * dt;
    if (step > 1) step = 1;
    lift_ += (goal - lift_) * step;

    layout();
}

// Puts a Text so it's centered left-to-right on centerX, with its top at y.
static void centerText(Text& text, double centerX, double y) {
    text.setPosition(Point(centerX - text.getWidth() / 2, y));
}

void UpgradeCard::layout() {
    double top = y_ - 14 * lift_;
    double centerX = x_ + WIDTH / 2.0;
    Point iconCenter(centerX, top + 80);
    Color color = getUpgradeColor(upgrade_);

    shadow_.setPosition(Point(x_ + 6, y_ + 8));
    background_.setPosition(Point(x_, top));
    background_.setOutlineColor(hovered_ ? color : Color(255, 255, 255, 50));
    band_.setPosition(Point(x_, top));

    iconOuter_.setPosition(iconCenter);
    iconOuter_.setRadius(44 + 4 * lift_);
    iconInner_.setPosition(iconCenter);
    centerText(iconLetter_, centerX + 2, iconCenter.y - iconLetter_.getHeight() / 2 + 2);

    centerText(title_, centerX, top + 145);
    title_.setColor(hovered_ ? color : Color::White);
    centerText(description_, centerX, top + 185);
    centerText(keyHint_, centerX, top + HEIGHT - 26);
}

void UpgradeCard::draw(Window& window) const {
    shadow_.draw(window);
    background_.draw(window);
    band_.draw(window);
    iconOuter_.draw(window);
    iconInner_.draw(window);
    iconLetter_.draw(window);
    title_.draw(window);
    description_.draw(window);
    keyHint_.draw(window);
}

bool UpgradeCard::wasChosen(const Window& window) const {
    // Ignore input until the card has (nearly) finished sliding in.
    bool arrived = y_ - targetY_ < 20;
    if (!arrived) return false;
    return window.wasKeyPressed('0' + keyNumber_) || (hovered_ && window.wasMouseClicked());
}

Upgrade UpgradeCard::getUpgrade() const { return upgrade_; }
