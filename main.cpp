// main.cpp - the smallest useful Graphics1300 program.
// Build:  build.bat main.cpp   (Windows)    or    bash build.sh main.cpp   (Mac/Linux)

#include "graphics/Window.h"
#include "graphics/Circle.h"
#include "graphics/Color.h"
#include "graphics/Line.h"
#include "graphics/Point.h"
#include "graphics/Rectangle.h"
#include "graphics/Text.h"
#include "graphics/Triangle.h"
#include <string>

int main() {
    Window window(640, 480, "Hello, Graphics1300");

    // Create the shapes once, before the game loop.
    Rectangle box(Point(60, 60), 200, 120, Color::Blue);
    box.setOutlineColor(Color::White);

    Circle sun(Point(420, 140), 70, Color::Orange);
    Triangle hill(Point(320, 420), Point(420, 260), Point(520, 420), Color::Green);
    Line beam(Point(40, 440), Point(600, 300), Color::Yellow);

    // A see-through rectangle (alpha 120 out of 255).
    Rectangle glass(Point(100, 140), 380, 80, Color(255, 0, 120, 120));

    Text hello("Hello, world!", Point(60, 260), Color::White, 3);
    Text mouse("", Point(60, 310), Color::LightGray);

    while (window.isOpen()) {
        mouse.setText("Mouse: " + std::to_string(window.getMouseX()) + ", "
                      + std::to_string(window.getMouseY()));

        // Clicking the sun changes its color.
        if (window.wasMouseClicked() && sun.contains(window.getMousePosition())) {
            sun.setColor(sun.getColor().r == 255 ? Color::Yellow : Color::Orange);
        }

        window.clear(Color(30, 30, 45));
        box.draw(window);
        sun.draw(window);
        hill.draw(window);
        beam.draw(window);
        glass.draw(window);
        hello.draw(window);
        mouse.draw(window);
        window.update();
    }
    return 0;
}
