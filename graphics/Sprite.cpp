#include "Sprite.h"
#include "Internal.h"

Sprite::Sprite(const Image& image, Point topLeft)
    : Sprite(image, topLeft, image.getWidth(), image.getHeight()) {}

Sprite::Sprite(const Image& image, Point topLeft, double width, double height)
    : image_(image), position_(topLeft), width_(width), height_(height),
      sourceX_(0), sourceY_(0),
      sourceWidth_(image.getWidth()), sourceHeight_(image.getHeight()) {}

void Sprite::draw(Window& window) const {
    Tigr* screen = LibraryAccess::screen(window);
    Tigr* src = LibraryAccess::pixels(image_);
    if (screen == nullptr || src == nullptr) return;
    int x = internal::roundToPixel(position_.x - window.getCameraX());
    int y = internal::roundToPixel(position_.y - window.getCameraY());
    int w = internal::roundToPixel(width_);
    int h = internal::roundToPixel(height_);

    if (w == sourceWidth_ && h == sourceHeight_) {
        tigrBlitAlpha(screen, src, x, y, sourceX_, sourceY_, sourceWidth_, sourceHeight_, 1.0f);
    } else {
        internal::blitScaled(screen, src, sourceX_, sourceY_, sourceWidth_, sourceHeight_,
                             x, y, w, h, 255);
    }
}

Point Sprite::getPosition() const { return position_; }
void Sprite::setPosition(Point topLeft) { position_ = topLeft; }
double Sprite::getX() const { return position_.x; }
double Sprite::getY() const { return position_.y; }
void Sprite::setX(double x) { position_.x = x; }
void Sprite::setY(double y) { position_.y = y; }
void Sprite::move(double dx, double dy) { position_.x += dx; position_.y += dy; }

double Sprite::getWidth() const  { return width_; }
double Sprite::getHeight() const { return height_; }
void Sprite::setSize(double width, double height) { width_ = width; height_ = height; }

void Sprite::setFrame(int sourceX, int sourceY, int sourceWidth, int sourceHeight) {
    sourceX_ = sourceX;
    sourceY_ = sourceY;
    sourceWidth_ = sourceWidth;
    sourceHeight_ = sourceHeight;
}

void Sprite::setImage(const Image& image) {
    image_ = image;
    setFrame(0, 0, image.getWidth(), image.getHeight());
}

bool Sprite::contains(Point point) const {
    return point.x >= position_.x && point.x < position_.x + width_ &&
           point.y >= position_.y && point.y < position_.y + height_;
}
