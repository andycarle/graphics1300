// Game.h - owns everything in the game and runs the game loop.
//
// The game is always in one of a few states (title screen, playing, paused,
// choosing an upgrade, ...). Each frame, run() calls the update method for
// the current state, then draws.
#pragma once

#include "graphics/Window.h"
#include "graphics/Circle.h"
#include "graphics/Color.h"
#include "graphics/Image.h"
#include "graphics/Line.h"
#include "graphics/Point.h"
#include "graphics/Rectangle.h"
#include "graphics/Sprite.h"
#include "graphics/Text.h"
#include "graphics/Triangle.h"
#include "Bullet.h"
#include "Coin.h"
#include "Enemy.h"
#include "FloatingText.h"
#include "Particle.h"
#include "Player.h"
#include "UpgradeCard.h"
#include <string>
#include <vector>

enum class GameState { Title, Playing, Paused, LevelUp, Dying, GameOver };

class Game {
public:
    explicit Game(Window& window);

    // Runs until the window is closed.
    void run();

private:
    // ---- Updating, one method per state ------------------------------------
    void updateTitle(double dt);
    void updatePlaying(double dt);
    void updatePaused();
    void updateLevelUp(double dt);
    void updateDying(double dt);
    void updateGameOver(double dt);

    // ---- Gameplay steps -----------------------------------------------------
    void startNewGame();
    void updateWaves();
    void spawnEnemies(double dt);
    void spawnEnemy(EnemyType type, Point start);
    void spawnAtEdge(EnemyType type);
    void spawnRingOfDrones();
    void fireAtNearestEnemy();
    void handleBulletHits();
    void defeatEnemy(int index);
    void handlePlayerHits();
    void startDying();
    void collectCoins();
    void openLevelUp();
    void keepEnemiesApart();
    void updateEffects(double dt);
    void removeFinishedObjects();

    // ---- Special effects ----------------------------------------------------
    void explode(Point center, Color color, int count, double speed);
    void shakeScreen(double amount);
    void flashScreen(Color color, double strength);
    void popup(const std::string& message, double centerX, double y, Color color, int scale);

    // ---- Drawing ------------------------------------------------------------
    void draw();
    void drawBackground();
    void drawWorld();
    void drawHud();
    void drawTitle();
    void drawLevelUp();
    void drawPaused();
    void drawGameOver();
    void drawCentered(Text& text, double y);   // centered, with a drop shadow

    Window& window_;
    Image slimeImage_;
    Image coinSheet_;
    GameState state_;

    // ---- Everything in the arena --------------------------------------------
    Player player_;
    std::vector<Enemy> enemies_;
    std::vector<Bullet> bullets_;
    std::vector<Coin> coins_;
    std::vector<Particle> particles_;
    std::vector<FloatingText> popups_;
    std::vector<UpgradeCard> cards_;

    // ---- Progress -----------------------------------------------------------
    double survivalTime_;
    int wave_;
    double spawnTimer_;
    int nextEnemyId_;
    int kills_;
    int level_;
    int coinsCollected_;
    int coinsNeeded_;      // coins needed for the next level
    double bestTime_;
    bool newBest_;
    double stateTimer_;    // seconds since the current state began

    // ---- Effects ------------------------------------------------------------
    double shake_;         // how far the screen shakes, in pixels
    double flashStrength_; // 0 = no flash, 1 = solid color
    Color flashColor_;
    double displayedHealth_;   // slides down after a hit, for the health bar

    // ---- Background ---------------------------------------------------------
    std::vector<Line> gridLines_;
    std::vector<Circle> pebbles_;
    std::vector<Rectangle> vignette_;

    // ---- Heads-up display ---------------------------------------------------
    Rectangle healthBack_;
    Rectangle healthChip_;
    Rectangle healthFill_;
    Text healthText_;
    Text levelText_;
    Text timeText_;
    Text waveText_;
    Text killsText_;
    Rectangle xpBack_;
    Rectangle xpFill_;

    // ---- Overlays and screens -----------------------------------------------
    Rectangle dim_;
    Rectangle flash_;
    Sprite titleSlime_;
    std::vector<Triangle> titleDrones_;
    Text titleSmall_;
    Text titleBig_;
    Text titlePrompt_;
    Text titleHelp_;
    Text titleBest_;
    Text levelUpTitle_;
    Text levelUpHint_;
    Text pausedTitle_;
    Text pausedHelp_;
    Text gameOverTitle_;
    Text gameOverStats_;
    Text gameOverBest_;
    Text gameOverPrompt_;
    Text gameOverHelp_;
};
