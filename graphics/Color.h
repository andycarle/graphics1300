// Color.h - an RGBA color.
#pragma once

class Color {
public:
    int r, g, b, a;   // each 0..255.  a = alpha (255 = solid, 0 = invisible)

    // Solid black.
    Color();

    // A color from red, green and blue (0..255), and optionally alpha.
    // Values outside 0..255 are treated as 0 or 255.
    Color(int red, int green, int blue, int alpha = 255);

    // Returns a copy of this color with a different alpha (transparency).
    Color withAlpha(int alpha) const;

    // Named colors, e.g. Color::Red.
    static const Color Black, White, Gray, LightGray, DarkGray,
                       Red, Green, Blue, Yellow, Orange, Purple,
                       Pink, Brown, Cyan, Magenta, Transparent;
};

inline const Color Color::Black       (  0,   0,   0);
inline const Color Color::White       (255, 255, 255);
inline const Color Color::Gray        (128, 128, 128);
inline const Color Color::LightGray   (200, 200, 200);
inline const Color Color::DarkGray    ( 64,  64,  64);
inline const Color Color::Red         (230,  41,  55);
inline const Color Color::Green       (  0, 200,  80);
inline const Color Color::Blue        (  0, 121, 241);
inline const Color Color::Yellow      (253, 224,  60);
inline const Color Color::Orange      (255, 161,   0);
inline const Color Color::Purple      (150,  80, 220);
inline const Color Color::Pink        (255, 128, 190);
inline const Color Color::Brown       (127,  90,  50);
inline const Color Color::Cyan        (  0, 220, 230);
inline const Color Color::Magenta     (255,   0, 255);
inline const Color Color::Transparent (  0,   0,   0,   0);
