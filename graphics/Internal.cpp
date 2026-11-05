#include "Internal.h"
#include <algorithm>
#include <cmath>

namespace internal {

static unsigned char clamp(int v) {
    return (unsigned char)(v < 0 ? 0 : (v > 255 ? 255 : v));
}

TPixel toTPixel(Color color) {
    return tigrRGBA(clamp(color.r), clamp(color.g), clamp(color.b), clamp(color.a));
}

TPixel toShapePixel(Color color) {
    // TIGR blends with (alpha / 256) squared. Passing 256 * sqrt(alpha / 255)
    // (minus TIGR's +1 rounding) makes the blend come out to alpha / 255.
    double fraction = clamp(color.a) / 255.0;
    int adjusted = (int)std::lround(std::sqrt(fraction) * 256) - 1;
    return tigrRGBA(clamp(color.r), clamp(color.g), clamp(color.b), clamp(adjusted));
}

int roundToPixel(double value) {
    return (int)std::lround(value);
}

void fillArea(Tigr* dest, int x, int y, int width, int height, TPixel color) {
    if (dest == nullptr || width <= 0 || height <= 0) return;
    // tigrFillRect fills only the inside of the rectangle; grow it by one
    // pixel on each side so the whole width x height area is covered.
    tigrFillRect(dest, x - 1, y - 1, width + 2, height + 2, color);
}

void blitScaled(Tigr* dest, Tigr* src, int sx, int sy, int sw, int sh,
                int dx, int dy, int dw, int dh, int alpha) {
    if (!dest || !src || sw <= 0 || sh <= 0 || dw <= 0 || dh <= 0) return;
    int startX = std::max(dx, 0), endX = std::min(dx + dw, dest->w);
    int startY = std::max(dy, 0), endY = std::min(dy + dh, dest->h);
    for (int y = startY; y < endY; y++) {
        int srcY = sy + (y - dy) * sh / dh;
        if (srcY < 0 || srcY >= src->h) continue;
        for (int x = startX; x < endX; x++) {
            int srcX = sx + (x - dx) * sw / dw;
            if (srcX < 0 || srcX >= src->w) continue;
            TPixel s = src->pix[srcY * src->w + srcX];
            int a = s.a * alpha / 255;
            if (a == 0) continue;
            TPixel& d = dest->pix[y * dest->w + x];
            d.r = (unsigned char)((s.r * a + d.r * (255 - a)) / 255);
            d.g = (unsigned char)((s.g * a + d.g * (255 - a)) / 255);
            d.b = (unsigned char)((s.b * a + d.b * (255 - a)) / 255);
            // d.a is left alone, so the window stays fully opaque.
        }
    }
}

}
