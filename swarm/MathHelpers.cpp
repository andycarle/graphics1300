#include "MathHelpers.h"
#include <cmath>
#include <random>

// One random number generator for the whole game, seeded differently every run.
static std::mt19937& generator() {
    static std::mt19937 gen(std::random_device{}());
    return gen;
}

double randomDouble(double min, double max) {
    std::uniform_real_distribution<double> dist(min, max);
    return dist(generator());
}

int randomInt(int min, int max) {
    std::uniform_int_distribution<int> dist(min, max);
    return dist(generator());
}

Point pointAtAngle(Point center, double angle, double distance) {
    return Point(center.x + std::cos(angle) * distance,
                 center.y + std::sin(angle) * distance);
}

std::string formatTime(double seconds) {
    int total = (int)seconds;
    int minutes = total / 60;
    int secs = total % 60;
    return std::to_string(minutes) + ":" + (secs < 10 ? "0" : "") + std::to_string(secs);
}
