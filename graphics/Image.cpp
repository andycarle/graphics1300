#include "Image.h"
#include "../tigr.h"
#include <iostream>

// A magenta-and-black checkerboard, shown in place of an image that failed to load.
static Tigr* makeMissingImage() {
    const int size = 32, square = 8;
    Tigr* bmp = tigrBitmap(size, size);
    for (int y = 0; y < size; y++) {
        for (int x = 0; x < size; x++) {
            bool magenta = ((x / square) + (y / square)) % 2 == 0;
            bmp->pix[y * size + x] = magenta ? tigrRGB(255, 0, 255) : tigrRGB(0, 0, 0);
        }
    }
    return bmp;
}

Image::Image() {}

Image::Image(const std::string& filename) {
    Tigr* loaded = tigrLoadImage(filename.c_str());
    if (loaded == nullptr) {
        std::cerr << "[graphics1300] Could not load image \"" << filename << "\".\n"
                  << "         Is the file in the same folder you ran the program from,\n"
                  << "         and is it a .png?\n";
        loaded = makeMissingImage();
    }
    pixels_ = std::shared_ptr<Tigr>(loaded, tigrFree);
}

int Image::getWidth() const  { return pixels_ ? pixels_->w : 0; }
int Image::getHeight() const { return pixels_ ? pixels_->h : 0; }

Color Image::getPixel(int x, int y) const {
    if (!pixels_ || x < 0 || y < 0 || x >= pixels_->w || y >= pixels_->h) {
        return Color::Transparent;
    }
    TPixel p = pixels_->pix[y * pixels_->w + x];
    return Color(p.r, p.g, p.b, p.a);
}
