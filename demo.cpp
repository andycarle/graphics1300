// demo.cpp - sprites, a sprite sheet, keyboard, mouse, and timing.
// Move the slime with the arrow keys or WASD and collect the coins.
// Click anywhere to drop a new coin.
// Build:  build.bat demo.cpp   (Windows)    or    bash build.sh demo.cpp   (Mac/Linux)

#include "graphics/Window.h"
#include "graphics/Color.h"
#include "graphics/Image.h"
#include "graphics/Point.h"
#include "graphics/Rectangle.h"
#include "graphics/Sprite.h"
#include "graphics/Text.h"
#include <string>
#include <vector>

const int SCALE = 4;           // draw the 16x16 sprites 4x bigger
const int SPRITE = 16 * SCALE; // on-screen sprite size

// A coin is a Sprite that animates through the 4 frames of coin.png.
class Coin {
public:
    Coin(const Image& sheet, Point topLeft) : sprite_(sheet, topLeft, SPRITE, SPRITE) {}

    void draw(Window& window) {
        // The sheet holds 4 frames side by side; pick one based on time.
        int frame = int(window.getTime() * 8) % 4;
        sprite_.setFrame(frame * 16, 0, 16, 16);
        sprite_.draw(window);
    }

    // True if this coin overlaps the given sprite.
    bool touches(const Sprite& other) const {
        return other.getX() < sprite_.getX() + SPRITE && sprite_.getX() < other.getX() + SPRITE &&
               other.getY() < sprite_.getY() + SPRITE && sprite_.getY() < other.getY() + SPRITE;
    }

private:
    Sprite sprite_;
};

int main() {
    Window window(800, 600, "Graphics1300 demo");
    Image slimeImage("slime.png");
    Image coinSheet("coin.png");

    Sprite player(slimeImage, Point(100, 300), SPRITE, SPRITE);
    const double speed = 300;   // pixels per second
    int score = 0;

    std::vector<Coin> coins = { Coin(coinSheet, Point(400, 120)),
                                Coin(coinSheet, Point(600, 400)),
                                Coin(coinSheet, Point(250, 470)) };

    Rectangle grass(Point(0, 540), 800, 60, Color(60, 120, 60));
    Text scoreText("", Point(16, 16), Color::Yellow, 3);
    Text help("Arrows/WASD to move, click to add a coin, Esc to quit",
              Point(16, 560), Color::White, 2);

    while (window.isOpen()) {
        // ---- Input and movement ------------------------------------------
        double step = speed * window.getDeltaTime();
        if (window.isKeyDown(Key::Left)  || window.isKeyDown('A')) player.move(-step, 0);
        if (window.isKeyDown(Key::Right) || window.isKeyDown('D')) player.move(step, 0);
        if (window.isKeyDown(Key::Up)    || window.isKeyDown('W')) player.move(0, -step);
        if (window.isKeyDown(Key::Down)  || window.isKeyDown('S')) player.move(0, step);

        if (window.wasMouseClicked()) {
            // Center the new coin on the mouse.
            Point mouse = window.getMousePosition();
            coins.push_back(Coin(coinSheet, Point(mouse.x - SPRITE / 2, mouse.y - SPRITE / 2)));
        }
        if (window.wasKeyPressed(Key::Escape)) {
            window.close();
            break;
        }

        // ---- Collect any coin the slime is touching ----------------------
        for (size_t i = 0; i < coins.size(); i++) {
            if (coins[i].touches(player)) {
                coins.erase(coins.begin() + i);
                i--;
                score++;
            }
        }
        scoreText.setText("Score: " + std::to_string(score));

        // ---- Draw --------------------------------------------------------
        window.clear(Color(40, 44, 70));
        grass.draw(window);
        for (Coin& coin : coins) {
            coin.draw(window);
        }
        player.draw(window);
        scoreText.draw(window);
        help.draw(window);

        window.update();
    }
    return 0;
}
