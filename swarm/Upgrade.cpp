#include "Upgrade.h"
#include "MathHelpers.h"
#include <utility>

std::string getUpgradeName(Upgrade upgrade) {
    switch (upgrade) {
        case Upgrade::RapidFire:   return "Rapid Fire";
        case Upgrade::Multishot:   return "Multishot";
        case Upgrade::Piercing:    return "Piercing";
        case Upgrade::HeavyRounds: return "Heavy Rounds";
        case Upgrade::SwiftFeet:   return "Swift Feet";
        case Upgrade::Magnet:      return "Magnet";
        case Upgrade::PatchUp:     return "Patch Up";
    }
    return "";
}

std::string getUpgradeDescription(Upgrade upgrade) {
    switch (upgrade) {
        case Upgrade::RapidFire:   return "Fire 20% faster.";
        case Upgrade::Multishot:   return "Shoot one more bullet\nin every volley.";
        case Upgrade::Piercing:    return "Bullets pass through\none more enemy.";
        case Upgrade::HeavyRounds: return "Bullets deal\n+1 damage.";
        case Upgrade::SwiftFeet:   return "Move 15% faster.";
        case Upgrade::Magnet:      return "Pull in coins from\nmuch farther away.";
        case Upgrade::PatchUp:     return "Heal completely and\ngain +1 max health.";
    }
    return "";
}

Color getUpgradeColor(Upgrade upgrade) {
    switch (upgrade) {
        case Upgrade::RapidFire:   return Color(255, 170, 40);
        case Upgrade::Multishot:   return Color(80, 200, 255);
        case Upgrade::Piercing:    return Color(200, 120, 255);
        case Upgrade::HeavyRounds: return Color(255, 80, 90);
        case Upgrade::SwiftFeet:   return Color(90, 230, 140);
        case Upgrade::Magnet:      return Color(253, 224, 60);
        case Upgrade::PatchUp:     return Color(255, 130, 190);
    }
    return Color::White;
}

std::vector<Upgrade> pickRandomUpgrades(int count) {
    // Shuffle the list of every upgrade, then take the first `count`.
    std::vector<Upgrade> all;
    for (int i = 0; i < UPGRADE_COUNT; i++) {
        all.push_back(static_cast<Upgrade>(i));
    }
    for (int i = (int)all.size() - 1; i > 0; i--) {
        std::swap(all[i], all[randomInt(0, i)]);
    }
    all.resize(count);
    return all;
}
