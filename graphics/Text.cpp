#include "Text.h"
#include "Internal.h"

Text::Text(const std::string& text, Point topLeft, Color color, int scale)
    : text_(text), position_(topLeft), color_(color), scale_(scale) {}

void Text::draw(Window& window) const {
    Tigr* screen = LibraryAccess::screen(window);
    if (screen == nullptr || text_.empty() || scale_ < 1) return;
    int x = internal::roundToPixel(position_.x - window.getCameraX());
    int y = internal::roundToPixel(position_.y - window.getCameraY());

    if (scale_ == 1) {
        tigrPrint(screen, tfont, x, y, internal::toTPixel(color_), "%s", text_.c_str());
        return;
    }
    // Bigger text: print at normal size into a scratch image, then enlarge it.
    int w = tigrTextWidth(tfont, text_.c_str());
    int h = tigrTextHeight(tfont, text_.c_str());
    Tigr* scratch = tigrBitmap(w, h);
    tigrClear(scratch, tigrRGBA(0, 0, 0, 0));
    tigrPrint(scratch, tfont, 0, 0, internal::toTPixel(color_.withAlpha(255)), "%s", text_.c_str());
    internal::blitScaled(screen, scratch, 0, 0, w, h, x, y, w * scale_, h * scale_, color_.a);
    tigrFree(scratch);
}

std::string Text::getText() const { return text_; }
void Text::setText(const std::string& text) { text_ = text; }

Point Text::getPosition() const { return position_; }
void Text::setPosition(Point topLeft) { position_ = topLeft; }
double Text::getX() const { return position_.x; }
double Text::getY() const { return position_.y; }
void Text::setX(double x) { position_.x = x; }
void Text::setY(double y) { position_.y = y; }
void Text::move(double dx, double dy) { position_.x += dx; position_.y += dy; }

int Text::getScale() const { return scale_; }
void Text::setScale(int scale) { scale_ = scale; }

double Text::getWidth() const {
    return text_.empty() ? 0 : tigrTextWidth(tfont, text_.c_str()) * scale_;
}

double Text::getHeight() const {
    return tigrTextHeight(tfont, text_.empty() ? " " : text_.c_str()) * scale_;
}

Color Text::getColor() const { return color_; }
void Text::setColor(Color color) { color_ = color; }
