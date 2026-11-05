// MathHelpers.h - small functions used all over the game.
#pragma once

#include "graphics/Point.h"
#include <string>

// A random decimal number from min up to max.
double randomDouble(double min, double max);

// A random whole number from min to max, including both.
int randomInt(int min, int max);

// The point `distance` away from `center` in the direction `angle` (radians).
// Angle 0 points right; angles grow clockwise, because y grows downward.
Point pointAtAngle(Point center, double angle, double distance);

// Seconds as "m:ss", e.g. 95.3 becomes "1:35".
std::string formatTime(double seconds);
