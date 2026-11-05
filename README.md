# Graphics1300: simple 2D graphics for C++

Graphics1300 gives you a window you can draw shapes, text and pictures on, and read the keyboard and mouse from. Everything you draw is an object: you create a `Rectangle`, `Circle`, `Text` or `Sprite`, change it with its methods, and call its `draw` method every frame.

This file gets you started. **[API.md](API.md)** lists every class and method.

## What's in this folder

| File | What it is |
|---|---|
| `graphics/` | The library's classes. Each one has a `.h` file (what the class can do, worth reading) and a `.cpp` file (how it does it, you don't need to understand these). |
| `API.md` | The reference: every class and method. |
| `tigr.c`, `tigr.h` | The small library Graphics1300 is built on. Don't edit these. |
| `build.bat` | Builds and runs your program on **Windows**. |
| `build.sh` | Builds and runs your program on **Mac** or **Linux** (or Git Bash on Windows). |
| `main.cpp` | A minimal starter program. |
| `demo.cpp` | A bigger example: sprites, animation, keyboard and mouse. |
| `slime.png`, `coin.png` | Images used by the demo. |
| `swarm/` | **Slime vs. the Swarm**, a complete game split into one class per file. |

You'll want to put your own `.cpp` files in this folder. Or you can put your files in a sub-directory, like the `swarm/` example.

## Building and running

Open a terminal **in this folder**, then:

**Windows** (Command Prompt or PowerShell):
```
build.bat main.cpp
```
(In PowerShell you may need to type `.\build.bat main.cpp`.)

**Mac or Linux:**
```
bash build.sh main.cpp
```

Replace `main.cpp` with whichever file you want to build, such as `demo.cpp`. The script compiles your program together with the library's classes and then runs it.

**Programs with several files:** put them in their own folder inside this one and give the folder name instead: `build.bat swarm` (or `bash build.sh swarm`). Every `.cpp` file in that folder is compiled, and the program runs from inside that folder, so its images belong there too. Your files include the library's classes the same way as always, for example `#include "graphics/Sprite.h"`.

The first build takes a little longer because it also compiles `tigr.c`. After that it reuses the result (`tigr-windows.o`, `tigr-mac.o` or `tigr-linux.o`).

### Building without the scripts

If you'd rather type the commands yourself, compile `tigr.c` once:
```
g++ -x c -c tigr.c -o tigr.o
```
Then build your program together with the library's classes in the `graphics` folder:
```
Windows:  g++ -std=c++17 main.cpp graphics/*.cpp tigr.o -o main.exe -static -lopengl32 -lgdi32
Mac:      g++ -std=c++17 main.cpp graphics/*.cpp tigr.o -o main -framework OpenGL -framework Cocoa
Linux:    g++ -std=c++17 main.cpp graphics/*.cpp tigr.o -o main -lGLU -lGL -lX11
```

## Your first program

```cpp
#include "graphics/Window.h"
#include "graphics/Circle.h"
#include "graphics/Color.h"
#include "graphics/Point.h"

int main() {
    Window window(640, 480, "My Program");          // width, height, title
    Circle ball(Point(320, 240), 50, Color::Red);   // center, radius, color

    while (window.isOpen()) {                       // the game loop
        ball.move(100 * window.getDeltaTime(), 0);  // 100 pixels per second

        window.clear(Color::Black);                 // 1. erase the last frame
        ball.draw(window);                          // 2. draw this frame
        window.update();                            // 3. show it and read input
    }
    return 0;
}
```

**Include each class you use.** Every Graphics1300 class has its own header in the `graphics` folder, and you `#include` the ones your file names, just like `#include <string>` or your own `#include "Book.h"`. This program names `Window`, `Circle`, `Color` and `Point`, so it includes those four. Sometimes a program compiles even when you forgot one, because one header can include another for its own use. Include it anyway: the rule is one include for every class you name.

Every Graphics1300 program has this shape. **Create your objects before the loop.** Then, about 60 times per second until the window is closed: **update** them, **clear** the window, **draw** them, and call `window.update()`.

**Coordinates:** `(0, 0)` is the **top-left** corner. `x` gets bigger going right, and `y` gets bigger going **down**. Positions are written as a `Point`, like `Point(320, 240)`.

## Next steps

**Move with the keyboard.** `isKeyDown` is true on every frame a key is held:
```cpp
double step = 200 * window.getDeltaTime();   // 200 pixels per second
if (window.isKeyDown(Key::Left))  ball.move(-step, 0);
if (window.isKeyDown(Key::Right)) ball.move(step, 0);
```
Multiplying by `getDeltaTime()` keeps the speed the same on fast and slow computers.

**React to clicks.** `wasMouseClicked` is true for one frame when a click finishes:
```cpp
if (window.wasMouseClicked() && ball.contains(window.getMousePosition())) {
    ball.setColor(Color::Yellow);
}
```

**Show text and numbers.** (Needs `#include "graphics/Text.h"` and `#include <string>`.)
```cpp
Text score("Score: 0", Point(10, 10), Color::White, 2);   // 2 = twice the normal size
// ...later, in the loop:
score.setText("Score: " + std::to_string(points));
score.draw(window);
```

**Draw pictures.** Load each image once, before the loop, and place it with a Sprite. (Needs `#include "graphics/Image.h"` and `#include "graphics/Sprite.h"`.)
```cpp
Image slimeImage("slime.png");
Sprite player(slimeImage, Point(100, 300), 64, 64);   // top-left, width, height
```

**See-through colors.** Add a fourth number, alpha, from 255 (solid) to 0 (invisible): `Color(255, 0, 0, 128)` is half see-through red.

From here, `demo.cpp` puts these together in a small game, and [API.md](API.md) has everything else: outlines, sprite sheets, the camera, and more.

---

## Slime vs. the Swarm

The `swarm` folder holds a complete arena survival game built with Graphics1300. Build it with `build.bat swarm` (or `bash build.sh swarm`).

Move with WASD or the arrow keys; the slime shoots the nearest enemy automatically. Defeated enemies drop coins, and collecting enough coins lets you choose one of three upgrades. A new wave starts every 25 seconds, bringing more and tougher enemies. P pauses.

Each class is in its own `.h`/`.cpp` pair:

| Class | What it is |
|---|---|
| `Game` | Owns everything and runs the game loop. Switches between the title screen, playing, paused, level-up and game-over states. |
| `Player` | The slime: movement, health, and the stats that upgrades improve. |
| `Enemy` | Drones (chase you), Darters (flash, then dash) and Brutes (slow and tough). |
| `Bullet`, `Coin` | Shots and dropped coins. |
| `Particle`, `FloatingText` | Visual effects: explosion sparks and messages like "WAVE 3". |
| `UpgradeCard`, `Upgrade` | The level-up cards and the upgrades they offer. |

The tuning numbers (wave length, bullet speed, and so on) are at the top of `Game.cpp`.

---

## Troubleshooting

**`g++` is not recognized / command not found.** g++ isn't installed or isn't on your PATH. Check by typing `g++ --version`.

**`'Sprite' was not declared in this scope` (or `'Text'`, `'Point'`, ...).** Your file uses a class without including its header. Add the include at the top, for example `#include "graphics/Sprite.h"`. [API.md](API.md) shows the include line for every class.

**`undefined reference to 'Window::Window(...)'` (or another library class).** The library's `.cpp` files weren't compiled with your program. Use the build script, or include `graphics/*.cpp` in your `g++` command.

**My image is a magenta checkerboard.** The file name is wrong or the file isn't in the folder you ran the program from. Check the spelling, including upper/lower case, and that it's a `.png`.

**`use of deleted function 'Window::Window(const Window&)'`.** You passed a Window to a function by value, which would try to copy the window. Pass it by reference instead: `void drawPlayer(Window& window)`.

**Nothing moves / my shape is drawn in the wrong place.** Make sure you create your shapes **before** the game loop. A shape created inside the loop is rebuilt every frame and forgets any changes you made to it.

**The window is bigger than the size I asked for.** If you ask for a small window, it's scaled up by a whole number (2x, 3x...) to fit your screen nicely. Your coordinates and the mouse position still use the size you asked for, so nothing in your code changes.

**Linux: errors about `X11/Xlib.h` or `GL/gl.h`.** Install the development libraries:
`sudo apt install g++ libx11-dev libgl-dev libglu1-mesa-dev`

**Mac: errors about `objc_msgSend`.** Delete `tigr-mac.o`, then build again with this one-time command first:
`g++ -x c -c tigr.c -o tigr-mac.o -DOBJC_OLD_DISPATCH_PROTOTYPES=1`

**I copied my folder to a different computer and it won't build.** Delete any `tigr-*.o` files and build again.

---

Graphics1300 is built on [TIGR](https://github.com/erkkah/tigr), which is in the public domain (see [TIGR License](https://github.com/erkkah/tigr/blob/master/UNLICENSE)).
