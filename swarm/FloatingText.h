// FloatingText.h - a message that drifts upward and fades out,
// like "WAVE 3" or "Multishot!".
#pragma once

#include "graphics/Window.h"
#include "graphics/Color.h"
#include "graphics/Text.h"
#include <string>

class FloatingText {
public:
    // The text is centered left-to-right on centerX, with its top at y.
    FloatingText(const std::string& message, double centerX, double y,
                 Color color, int scale, double lifetime);

    void update(double dt);
    void draw(Window& window) const;
    bool isAlive() const;

private:
    Text text_;
    Text shadow_;
    Color color_;
    double age_, lifetime_;
};
