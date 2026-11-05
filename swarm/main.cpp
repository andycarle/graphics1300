// Slime vs. the Swarm
//
// Survive as long as you can. The slime shoots automatically; you steer.
// Defeated enemies drop coins, and coins level you up.
//
// Build and run from the G2 folder:
//     build.bat swarm          (Windows)
//     bash build.sh swarm      (Mac/Linux)

#include "graphics/Window.h"
#include "Game.h"
#include "Settings.h"

int main() {
    Window window(SCREEN_WIDTH, SCREEN_HEIGHT, "Slime vs. the Swarm");
    Game game(window);
    game.run();
    return 0;
}
