// Image.h - a PNG picture loaded from a file.
//
// An Image is just the picture. To put it on the screen, make a Sprite.
// Load each image once, before the game loop; many Sprites can share it.
// Copying an Image is cheap: copies share the same pixels.
#pragma once

#include "Color.h"
#include <memory>
#include <string>

struct Tigr;   // TIGR's picture type (used inside the library only)

class Image {
public:
    // An empty image. Drawing it does nothing.
    Image();

    // Loads a PNG file. If the file can't be loaded, prints an error and
    // uses a magenta-and-black checkerboard so the problem is easy to see.
    explicit Image(const std::string& filename);

    int getWidth() const;
    int getHeight() const;

    // Color of one pixel. Returns Color::Transparent if (x, y) is outside the image.
    Color getPixel(int x, int y) const;

private:
    std::shared_ptr<Tigr> pixels_;

    friend struct LibraryAccess;   // lets Sprite draw the pixels
};
