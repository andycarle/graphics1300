// Internal.h - helpers shared by the library's .cpp files.
// Students don't need to read this file.
#pragma once

#include "../tigr.h"
#include "Window.h"
#include "Color.h"
#include "Image.h"

// Gives the library's own drawing code the TIGR data hidden inside Window
// and Image. Both classes declare `friend struct LibraryAccess;`.
struct LibraryAccess {
    static Tigr* screen(const Window& window) { return window.screen_; }
    static Tigr* pixels(const Image& image)   { return image.pixels_.get(); }
};

namespace internal {

// Converts a Color to TIGR's pixel type, clamping each part to 0..255.
TPixel toTPixel(Color color);

// Like toTPixel, for TIGR's shape functions (tigrPlot, tigrLine, tigrRect,
// tigrFillRect, tigrCircle, tigrFillCircle). Those square the alpha, so
// alpha 128 would only be 25% opaque; this adjusts it so 128 means 50%.
TPixel toShapePixel(Color color);

// Rounds a coordinate to the nearest whole pixel.
int roundToPixel(double value);

// Fills exactly the width x height area whose top-left corner is (x, y).
void fillArea(Tigr* dest, int x, int y, int width, int height, TPixel color);

// Stretches part of src (sx, sy, sw, sh) onto dest (dx, dy, dw, dh) using
// nearest-neighbor scaling, so pixel art stays crisp. Blends using each
// pixel's alpha times the extra `alpha` (0..255).
void blitScaled(Tigr* dest, Tigr* src, int sx, int sy, int sw, int sh,
                int dx, int dy, int dw, int dh, int alpha);

}
