#include "Window.h"
#include "Internal.h"
#include <algorithm>
#include <iostream>

// Converts a key from our names (Key::Left, 'w', ...) to TIGR's codes.
static int toTigrKey(int key) {
    switch (key) {
        case Key::Left:      return TK_LEFT;
        case Key::Right:     return TK_RIGHT;
        case Key::Up:        return TK_UP;
        case Key::Down:      return TK_DOWN;
        case Key::Space:     return TK_SPACE;
        case Key::Enter:     return TK_RETURN;
        case Key::Escape:    return TK_ESCAPE;
        case Key::Backspace: return TK_BACKSPACE;
        case Key::Tab:       return TK_TAB;
        case Key::Shift:     return TK_SHIFT;
        case Key::Control:   return TK_CONTROL;
        case Key::Alt:       return TK_ALT;
    }
    // TIGR uses upper-case letters, so isKeyDown('w') works like 'W'.
    if (key >= 'a' && key <= 'z') return key - 'a' + 'A';
    return key;
}

Window::Window(int width, int height, const std::string& title) {
    screen_ = tigrWindow(width, height, title.c_str(), TIGR_FIXED);
    if (screen_ == nullptr) {
        std::cerr << "[graphics1300] Could not open a window.\n";
        return;
    }
    tigrClear(screen_, tigrRGB(0, 0, 0));
    // Keep the window fully opaque when see-through colors are drawn on it,
    // so screenshots look exactly like the screen.
    tigrBlitMode(screen_, TIGR_KEEP_ALPHA);
    tigrTime();   // start the frame timer
    tigrMouse(screen_, &mouseX_, &mouseY_, &mouseButtons_);
}

Window::~Window() { close(); }

bool Window::isOpen() const {
    return screen_ != nullptr && !tigrClosed(screen_);
}

void Window::update() {
    if (screen_ == nullptr) return;
    tigrUpdate(screen_);
    deltaTime_ = std::min(tigrTime(), 0.25f);   // cap long pauses
    totalTime_ += deltaTime_;
    previousMouseButtons_ = mouseButtons_;
    tigrMouse(screen_, &mouseX_, &mouseY_, &mouseButtons_);
}

void Window::close() {
    if (screen_ != nullptr) {
        tigrFree(screen_);
        screen_ = nullptr;
    }
}

int Window::getWidth() const  { return screen_ ? screen_->w : 0; }
int Window::getHeight() const { return screen_ ? screen_->h : 0; }

double Window::getDeltaTime() const { return deltaTime_; }
double Window::getTime() const      { return totalTime_; }

void Window::clear(Color color) {
    if (screen_ == nullptr) return;
    if (color.a >= 255) {
        tigrClear(screen_, internal::toTPixel(color));
    } else {
        // A see-through clear only partly covers the last frame,
        // which leaves fading trails behind moving things.
        internal::fillArea(screen_, 0, 0, screen_->w, screen_->h, internal::toShapePixel(color));
    }
}

bool Window::saveScreenshot(const std::string& filename) const {
    return screen_ != nullptr && tigrSaveImage(filename.c_str(), screen_) != 0;
}

void Window::setCamera(double cameraX, double cameraY) {
    cameraX_ = cameraX;
    cameraY_ = cameraY;
}

double Window::getCameraX() const { return cameraX_; }
double Window::getCameraY() const { return cameraY_; }

bool Window::isKeyDown(int key) const {
    return screen_ != nullptr && tigrKeyHeld(screen_, toTigrKey(key)) != 0;
}

bool Window::wasKeyPressed(int key) const {
    return screen_ != nullptr && tigrKeyDown(screen_, toTigrKey(key)) != 0;
}

Point Window::getMousePosition() const { return Point(mouseX_, mouseY_); }
int Window::getMouseX() const { return mouseX_; }
int Window::getMouseY() const { return mouseY_; }

bool Window::isMouseDown() const {
    return (mouseButtons_ & 1) != 0;
}

bool Window::wasMouseClicked() const {
    // Down on the previous frame, up on this one.
    return (previousMouseButtons_ & 1) != 0 && (mouseButtons_ & 1) == 0;
}
