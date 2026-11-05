# Graphics1300 API reference

Every Graphics1300 class, with the line that includes it. Include the header for each class your file names. For building and a first program, see [README.md](README.md). Each class's `.h` file in the `graphics` folder has the same information as comments.

**Coordinates:** `(0, 0)` is the **top-left** corner of the window. `x` gets bigger going right, and `y` gets bigger going **down**. Positions and sizes are in pixels and are `double`s, so small movements each frame add up correctly.

- [Window](#window): the game loop, keyboard, mouse and camera
- [Color](#color)
- [Point](#point)
- Shapes: [Rectangle](#rectangle), [Circle](#circle), [Line](#line), [Triangle](#triangle)
- [Text](#text)
- [Image](#image) and [Sprite](#sprite)
- [Keys](#keys)

---

## Window

`#include "graphics/Window.h"`

The window you draw in. It runs the game loop and reads the keyboard and mouse.

```cpp
Window window(640, 480, "My Program");   // width, height, title
```

A Window can't be copied. Pass it to functions by reference: `void drawPlayer(Window& window)`.

### The game loop

| Method | Does |
|---|---|
| `Window(int width, int height, std::string title)` | Opens a window `width` x `height` pixels. The title is optional. |
| `bool isOpen()` | `true` until the window is closed. |
| `void clear(Color color)` | Fills the whole window with one color (black if you leave it off). A see-through color only partly covers the last frame, which leaves fading trails behind moving things. |
| `void update()` | Shows what you drew and reads new input. Call it once at the end of every frame. |
| `void close()` | Closes the window. `isOpen()` returns `false` afterward. |
| `int getWidth()`, `int getHeight()` | The window size in pixels. |
| `double getDeltaTime()` | Seconds since the previous frame, about 0.0167 at 60 frames per second. |
| `double getTime()` | Seconds since the Window was created. |
| `bool saveScreenshot(std::string filename)` | Saves what's in the window as a PNG. Returns `true` if it worked. |

**Smooth movement:** multiply speeds by `getDeltaTime()`. Then `player.move(200 * window.getDeltaTime(), 0);` moves 200 pixels per second no matter how fast the computer is.

### Keyboard

| Method | `true` when |
|---|---|
| `bool isKeyDown(int key)` | The key is held down, on every frame it's held. Use for movement. |
| `bool wasKeyPressed(int key)` | The key was **just** pressed (one frame only). Use for jumping, menus and shooting. |

See [Keys](#keys) for what to pass as `key`.

### Mouse

| Method | Gives |
|---|---|
| `Point getMousePosition()` | The mouse position, in window pixels. |
| `int getMouseX()`, `int getMouseY()` | Just one part of the mouse position. |
| `bool isMouseDown()` | `true` while the left button is held down. |
| `bool wasMouseClicked()` | `true` only on the frame a click finishes: the left button was pressed and has just been let go. |

**Clickable things:** combine `wasMouseClicked` with `contains`:
```cpp
if (window.wasMouseClicked() && button.contains(window.getMousePosition())) {
    // the button was clicked
}
```

### Camera

| Method | Does |
|---|---|
| `void setCamera(double x, double y)` | Shifts everything drawn afterward: a shape at `(x, y)` appears on screen at `(x - cameraX, y - cameraY)`. |
| `double getCameraX()`, `double getCameraY()` | The current camera position. It starts at `(0, 0)`. |

Use the camera to scroll around a world bigger than the window, or to shake the screen by setting small random values for a few frames. Call `window.setCamera(0, 0)` before drawing things that should stay put, like the score. The mouse position is always in screen pixels, not world pixels.

---

## Color

`#include "graphics/Color.h"`

A color made of red, green, blue and alpha (transparency), each from 0 to 255.

| | |
|---|---|
| `Color(int r, int g, int b)` | A solid color. |
| `Color(int r, int g, int b, int a)` | `a` is alpha: 255 is solid, 128 is half see-through, 0 is invisible. |
| `Color withAlpha(int a)` | A copy of this color with a different alpha. |
| `r`, `g`, `b`, `a` | The parts, which you can read or change directly: `color.r = 255;` |

Values outside 0 to 255 are treated as 0 or 255.

Named colors: `Color::Black`, `White`, `Gray`, `LightGray`, `DarkGray`, `Red`, `Green`, `Blue`, `Yellow`, `Orange`, `Purple`, `Pink`, `Brown`, `Cyan`, `Magenta`, `Transparent`.

---

## Point

`#include "graphics/Point.h"`

A position in the window: an `x` and a `y`.

| | |
|---|---|
| `Point(double x, double y)` | The point `(x, y)`. |
| `Point()` | The point `(0, 0)`. |
| `x`, `y` | The parts, which you can read or change directly: `corner.x += 10;` |
| `double distanceTo(Point other)` | The straight-line distance to another point. |

You can also write a Point as `{60, 60}` wherever one is expected, for example `Rectangle box({60, 60}, 200, 120, Color::Blue);`.

---

## Shapes

Every shape follows the same pattern: create it once **before** the game loop, change it with its methods, and call `draw(window)` every frame.

For an **outline with no fill**, make the color `Color::Transparent` and set an outline color:
```cpp
Rectangle frame(Point(10, 10), 100, 50, Color::Transparent);
frame.setOutlineColor(Color::White);
```

### Rectangle

`#include "graphics/Rectangle.h"`

```cpp
Rectangle box(Point(60, 60), 200, 120, Color::Blue);   // top-left, width, height, color
```

| Method | Does |
|---|---|
| `Rectangle(Point topLeft, double width, double height, Color color)` | Makes a rectangle. |
| `void draw(Window& window)` | Draws it. |
| `Point getPosition()`, `void setPosition(Point topLeft)` | The top-left corner. |
| `double getX()`, `double getY()`, `void setX(double)`, `void setY(double)` | Just one part of the top-left corner. |
| `void move(double dx, double dy)` | Moves it by `(dx, dy)`. |
| `double getWidth()`, `double getHeight()`, `void setSize(double width, double height)` | Its size. |
| `Color getColor()`, `void setColor(Color)` | The fill color. |
| `Color getOutlineColor()`, `void setOutlineColor(Color)` | The outline color. The default, `Color::Transparent`, means no outline. |
| `bool contains(Point point)` | `true` if the point is inside the rectangle. |

### Circle

`#include "graphics/Circle.h"`

```cpp
Circle sun(Point(420, 140), 70, Color::Orange);   // center, radius, color
```

| Method | Does |
|---|---|
| `Circle(Point center, double radius, Color color)` | Makes a circle. |
| `void draw(Window& window)` | Draws it. |
| `Point getPosition()`, `void setPosition(Point center)` | The **center**. |
| `double getX()`, `double getY()`, `void setX(double)`, `void setY(double)` | Just one part of the center. |
| `void move(double dx, double dy)` | Moves it by `(dx, dy)`. |
| `double getRadius()`, `void setRadius(double)` | Its radius. |
| `Color getColor()`, `void setColor(Color)` | The fill color. |
| `Color getOutlineColor()`, `void setOutlineColor(Color)` | The outline color. The default, `Color::Transparent`, means no outline. |
| `bool contains(Point point)` | `true` if the point is inside the circle. |

### Line

`#include "graphics/Line.h"`

```cpp
Line beam(Point(40, 440), Point(600, 300), Color::Yellow);   // start, end, color
```

| Method | Does |
|---|---|
| `Line(Point start, Point end, Color color)` | Makes a line. |
| `void draw(Window& window)` | Draws it. |
| `Point getStart()`, `void setStart(Point)` | One end. |
| `Point getEnd()`, `void setEnd(Point)` | The other end. |
| `void move(double dx, double dy)` | Moves both ends by `(dx, dy)`. |
| `Color getColor()`, `void setColor(Color)` | Its color. |

### Triangle

`#include "graphics/Triangle.h"`

```cpp
Triangle hill(Point(320, 420), Point(420, 260), Point(520, 420), Color::Green);
```

| Method | Does |
|---|---|
| `Triangle(Point corner1, Point corner2, Point corner3, Color color)` | Makes a triangle. The corners can be in any order. |
| `void draw(Window& window)` | Draws it. |
| `Point getCorner1()`, `getCorner2()`, `getCorner3()` | The corners. |
| `void setCorners(Point corner1, Point corner2, Point corner3)` | Moves all three corners at once. |
| `void move(double dx, double dy)` | Moves the whole triangle by `(dx, dy)`. |
| `Color getColor()`, `void setColor(Color)` | The fill color. |
| `Color getOutlineColor()`, `void setOutlineColor(Color)` | The outline color. The default, `Color::Transparent`, means no outline. |

---

## Text

`#include "graphics/Text.h"`

Words drawn in the built-in pixel font.

```cpp
Text score("Score: 0", Point(10, 10), Color::White, 2);   // text, top-left, color, scale
score.setText("Score: " + std::to_string(points));
score.draw(window);
```

| Method | Does |
|---|---|
| `Text(std::string text, Point topLeft, Color color, int scale)` | Makes text. `scale` 2 is twice the normal size, 3 is three times, and so on. Leave it off for normal size. |
| `void draw(Window& window)` | Draws it. |
| `std::string getText()`, `void setText(std::string)` | The words. Use `\n` for a new line. |
| `Point getPosition()`, `void setPosition(Point topLeft)` | The top-left corner. |
| `double getX()`, `double getY()`, `void setX(double)`, `void setY(double)` | Just one part of the top-left corner. |
| `void move(double dx, double dy)` | Moves it by `(dx, dy)`. |
| `int getScale()`, `void setScale(int)` | Its size multiplier. |
| `double getWidth()`, `double getHeight()` | Its size on screen in pixels. Useful for centering. |
| `Color getColor()`, `void setColor(Color)` | Its color. |

To show a number, convert it to a string with `std::to_string`.

**Centering text:** `title.setX((window.getWidth() - title.getWidth()) / 2);`

---

## Image

`#include "graphics/Image.h"`

A picture loaded from a `.png` file. An Image is just the picture; to put it on the screen, make a [Sprite](#sprite).

```cpp
Image slimeImage("slime.png");   // load once, BEFORE the game loop
```

| Method | Does |
|---|---|
| `Image(std::string filename)` | Loads a PNG. If it can't be loaded, prints a message in the terminal and uses a magenta-and-black checkerboard instead. |
| `int getWidth()`, `int getHeight()` | Its size in pixels. |
| `Color getPixel(int x, int y)` | The color of one pixel, or `Color::Transparent` if `(x, y)` is outside the image. |

Copying an Image is cheap: copies share the same pixels. Many Sprites can share one Image.

---

## Sprite

`#include "graphics/Sprite.h"`

An [Image](#image) placed on the screen.

```cpp
Sprite player(slimeImage, Point(100, 300));             // normal size, top-left at (100, 300)
Sprite bigSlime(slimeImage, Point(300, 300), 64, 64);   // stretched to 64 x 64
player.draw(window);
```

| Method | Does |
|---|---|
| `Sprite(Image image, Point topLeft)` | Shows the whole image at its normal size. |
| `Sprite(Image image, Point topLeft, double width, double height)` | Shows the whole image stretched to `width` x `height`. |
| `void draw(Window& window)` | Draws it. |
| `Point getPosition()`, `void setPosition(Point topLeft)` | The top-left corner. |
| `double getX()`, `double getY()`, `void setX(double)`, `void setY(double)` | Just one part of the top-left corner. |
| `void move(double dx, double dy)` | Moves it by `(dx, dy)`. |
| `double getWidth()`, `double getHeight()`, `void setSize(double width, double height)` | Its size on screen. |
| `void setFrame(int sx, int sy, int sw, int sh)` | Shows only part of the image: the rectangle at `(sx, sy)` with size `sw` x `sh`. |
| `void setImage(Image image)` | Switches to a different image, showing all of it. The size on screen stays the same. |
| `bool contains(Point point)` | `true` if the point is inside the sprite's rectangle. |

**Sprite sheets** are images holding several animation frames side by side. `coin.png` holds four 16x16 frames, so frame `f` is `coin.setFrame(f * 16, 0, 16, 16);`. See `demo.cpp`.

Stretched sprites keep sharp pixel edges, which is good for pixel art.

---

## Keys

For `isKeyDown` and `wasKeyPressed`. The `Key::` names come with `#include "graphics/Window.h"`.

- **Letters and digits:** use the character, like `'W'` or `'7'`. Upper and lower case both work.
- **Other keys:** `Key::Left`, `Key::Right`, `Key::Up`, `Key::Down`, `Key::Space`, `Key::Enter`, `Key::Escape`, `Key::Backspace`, `Key::Tab`, `Key::Shift`, `Key::Control`, `Key::Alt`.
