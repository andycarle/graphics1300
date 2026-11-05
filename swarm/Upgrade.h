// Upgrade.h - the power-ups offered when the player levels up.
#pragma once

#include "graphics/Color.h"
#include <string>
#include <vector>

enum class Upgrade {
    RapidFire,
    Multishot,
    Piercing,
    HeavyRounds,
    SwiftFeet,
    Magnet,
    PatchUp
};

const int UPGRADE_COUNT = 7;

std::string getUpgradeName(Upgrade upgrade);
std::string getUpgradeDescription(Upgrade upgrade);
Color getUpgradeColor(Upgrade upgrade);

// `count` different upgrades, chosen at random.
std::vector<Upgrade> pickRandomUpgrades(int count);
