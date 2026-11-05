// Window.h - the window you draw on and read input from.
//
// Coordinates are in pixels: (0, 0) is the top-left corner,
// x grows to the right and y grows DOWNWARD.
//
// Every program has the same game loop:
//     while (window.isOpen()) {
//         window.clear(Color::Black);   // 1. erase the last frame
//         someShape.draw(window);       // 2. draw this frame
//         window.update();              // 3. show it and read input
//     }
//
// A Window can't be copied. Pass it to functions by reference:
//     void drawPlayer(Window& window) { ... }
#pragma once

#include "Color.h"
#include "Point.h"
#include <string>

struct Tigr;   // TIGR's window type (used inside the library only)

// Key names for isKeyDown and wasKeyPressed.
// For letters and digits just use the character: isKeyDown('W'), isKeyDown('3')
namespace Key {
    enum : int {
        Left = 1000, Right, Up, Down,
        Space, Enter, Escape, Backspace, Tab,
        Shift, Control, Alt
    };
}

class Window {
public:
    // Opens a window that is width x height pixels.
    Window(int width, int height, const std::string& title = "Graphics1300");
    ~Window();

    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;

    // ---- The game loop -----------------------------------------------------

    // True until the user closes the window (or you call close()).
    bool isOpen() const;

    // Shows everything drawn this frame and reads new keyboard/mouse input.
    // Call it once at the end of every pass through your loop.
    void update();

    // Closes the window. isOpen() returns false afterward.
    void close();

    int getWidth() const;
    int getHeight() const;

    // Seconds since the previous update() (about 0.0167 at 60 frames/sec).
    // Multiply speeds by this for smooth movement: x += speed * getDeltaTime();
    double getDeltaTime() const;

    // Seconds since the Window was created.
    double getTime() const;

    // Fills the whole window with one color. A see-through color only partly
    // covers the last frame, leaving fading trails behind moving things.
    void clear(Color color = Color::Black);

    // Saves what's currently in the window as a PNG. Returns true on success.
    bool saveScreenshot(const std::string& filename) const;

    // ---- Camera ------------------------------------------------------------

    // Shifts everything drawn afterward: a shape at (x, y) appears on the
    // screen at (x - cameraX, y - cameraY). Use it to scroll around a world
    // bigger than the window, or to shake the screen. Set it back to (0, 0)
    // before drawing things that should stay put, like the score.
    void setCamera(double cameraX, double cameraY);
    double getCameraX() const;
    double getCameraY() const;

    // ---- Keyboard ----------------------------------------------------------

    // True for every frame the key is held down. Use for movement.
    // Use Key::Left, Key::Space, etc., or a character like 'W' or '5'.
    bool isKeyDown(int key) const;

    // True only on the one frame the key was first pressed.
    // Use for jumping, menus and shooting.
    bool wasKeyPressed(int key) const;

    // ---- Mouse -------------------------------------------------------------

    // Where the mouse is, in window pixels.
    Point getMousePosition() const;
    int getMouseX() const;
    int getMouseY() const;

    // True while the left mouse button is held down.
    bool isMouseDown() const;

    // True only on the one frame a click finishes: the left mouse button
    // was pressed and has just been let go.
    bool wasMouseClicked() const;

private:
    Tigr*  screen_ = nullptr;
    double deltaTime_ = 0.0;
    double totalTime_ = 0.0;
    double cameraX_ = 0.0, cameraY_ = 0.0;
    int    mouseX_ = 0, mouseY_ = 0;
    int    mouseButtons_ = 0, previousMouseButtons_ = 0;

    friend struct LibraryAccess;   // lets the library's shapes draw on the window
};
