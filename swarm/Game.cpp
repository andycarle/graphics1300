#include "Game.h"
#include "MathHelpers.h"
#include "Settings.h"
#include <cmath>

// ---- Tuning: change these to change how the game plays ---------------------
const double WAVE_LENGTH = 25;      // seconds per wave
const int MAX_ENEMIES = 220;
const double BULLET_SPEED = 640;
const double TARGET_RANGE = 430;    // how far away the slime will shoot
const double BULLET_SPREAD = 0.16;  // radians between multishot bullets
const int GRID_SIZE = 48;
const int MAX_PARTICLES = 1500;

// Background ground color.
const Color GROUND(20, 18, 34);

Game::Game(Window& window)
    : window_(window),
      slimeImage_("slime.png"),
      coinSheet_("coin.png"),
      state_(GameState::Title),
      player_(slimeImage_),
      survivalTime_(0), wave_(1), spawnTimer_(0), nextEnemyId_(0),
      kills_(0), level_(1), coinsCollected_(0), coinsNeeded_(5),
      bestTime_(0), newBest_(false), stateTimer_(0),
      shake_(0), flashStrength_(0), flashColor_(Color::White), displayedHealth_(0),
      healthBack_(Point(18, 18), 224, 22, Color(0, 0, 0, 150)),
      healthChip_(Point(20, 20), 220, 18, Color(255, 240, 240)),
      healthFill_(Point(20, 20), 220, 18, Color(90, 230, 120)),
      healthText_("", Point(28, 25), Color::White),
      levelText_("", Point(20, 50), Color(253, 224, 60), 2),
      timeText_("", Point(), Color::White, 3),
      waveText_("", Point(), Color::LightGray),
      killsText_("", Point(0, 20), Color::White, 2),
      xpBack_(Point(0, SCREEN_HEIGHT - 8), SCREEN_WIDTH, 8, Color(0, 0, 0, 150)),
      xpFill_(Point(0, SCREEN_HEIGHT - 8), 0, 8, Color(253, 224, 60)),
      dim_(Point(), SCREEN_WIDTH, SCREEN_HEIGHT, Color(10, 8, 20, 170)),
      flash_(Point(), SCREEN_WIDTH, SCREEN_HEIGHT, Color::Transparent),
      titleSlime_(slimeImage_, Point(), 96, 96),
      titleSmall_("SLIME  vs.", Point(), Color::White, 3),
      titleBig_("THE SWARM", Point(), Color::Red, 8),
      titlePrompt_("Press SPACE or click to start", Point(), Color::White, 2),
      titleHelp_("WASD or arrow keys to move  -  you shoot automatically  -  P to pause",
                 Point(), Color::Gray),
      titleBest_("", Point(), Color(253, 224, 60)),
      levelUpTitle_("LEVEL UP!", Point(), Color(253, 224, 60), 5),
      levelUpHint_("Choose an upgrade", Point(), Color::LightGray, 2),
      pausedTitle_("PAUSED", Point(), Color::White, 6),
      pausedHelp_("P to resume    Q to quit to the title screen", Point(), Color::LightGray, 2),
      gameOverTitle_("THE SWARM WINS", Point(), Color(255, 80, 100), 6),
      gameOverStats_("", Point(), Color::White, 2),
      gameOverBest_("", Point(), Color(253, 224, 60), 2),
      gameOverPrompt_("Press SPACE or click to try again", Point(), Color::White, 2),
      gameOverHelp_("Esc for the title screen", Point(), Color::Gray) {

    // Grid lines for the floor (positioned every frame in drawBackground).
    int columns = SCREEN_WIDTH / GRID_SIZE + 6;
    int rows = SCREEN_HEIGHT / GRID_SIZE + 6;
    for (int i = 0; i < columns + rows; i++) {
        bool major = (i % 4 == 0);
        gridLines_.push_back(Line(Point(), Point(), Color(140, 120, 255, major ? 30 : 14)));
    }

    // Faint specks scattered on the floor.
    for (int i = 0; i < 70; i++) {
        Point spot(randomDouble(0, SCREEN_WIDTH), randomDouble(0, SCREEN_HEIGHT));
        pebbles_.push_back(Circle(spot, randomDouble(1, 3), Color(255, 255, 255, randomInt(8, 22))));
    }

    // Darken the edges of the screen: stacked see-through bands get
    // darker toward the edge. The corners overlap, so they're darkest.
    const int bands = 10, thickness = 6;
    for (int i = 0; i < bands; i++) {
        Color shade(0, 0, 0, 60 - i * 6);
        int inset = i * thickness;
        int far = SCREEN_WIDTH - inset - thickness;     // right band's x
        int low = SCREEN_HEIGHT - inset - thickness;    // bottom band's y
        vignette_.push_back(Rectangle(Point(0, inset), SCREEN_WIDTH, thickness, shade));
        vignette_.push_back(Rectangle(Point(0, low), SCREEN_WIDTH, thickness, shade));
        vignette_.push_back(Rectangle(Point(inset, 0), thickness, SCREEN_HEIGHT, shade));
        vignette_.push_back(Rectangle(Point(far, 0), thickness, SCREEN_HEIGHT, shade));
    }

    // Drones that circle the slime on the title screen.
    for (int i = 0; i < 9; i++) {
        Triangle drone(Point(), Point(), Point(), Color(255, 80, 100));
        drone.setOutlineColor(Color(255, 255, 255, 110));
        titleDrones_.push_back(drone);
    }

    healthBack_.setOutlineColor(Color(255, 255, 255, 70));
}

// =============================================================================
//  The game loop
// =============================================================================

void Game::run() {
    while (window_.isOpen()) {
        double dt = window_.getDeltaTime();

        switch (state_) {
            case GameState::Title:    updateTitle(dt); break;
            case GameState::Playing:  updatePlaying(dt); break;
            case GameState::Paused:   updatePaused(); break;
            case GameState::LevelUp:  updateLevelUp(dt); break;
            case GameState::Dying:    updateDying(dt); break;
            case GameState::GameOver: updateGameOver(dt); break;
        }
        if (!window_.isOpen()) break;   // the title screen can close the window

        draw();
        window_.update();
    }
}

// =============================================================================
//  Updating each state
// =============================================================================

void Game::updateTitle(double dt) {
    stateTimer_ += dt;

    if (window_.wasKeyPressed(Key::Escape)) {
        window_.close();
        return;
    }
    if (window_.wasKeyPressed(Key::Space) || window_.wasKeyPressed(Key::Enter) ||
        window_.wasMouseClicked()) {
        startNewGame();
        return;
    }

    // Drones fly in a tilted circle (an oval) around the slime.
    Point center(SCREEN_WIDTH / 2.0, 390);
    for (size_t i = 0; i < titleDrones_.size(); i++) {
        double angle = stateTimer_ * 0.8 + i * 2 * PI / titleDrones_.size();
        double radius = 150 + 12 * std::sin(stateTimer_ * 3 + i);
        Point drone(center.x + std::cos(angle) * radius,
                    center.y + std::sin(angle) * radius * 0.5);   // squashed into an oval
        double facing = angle + PI / 2;   // point the way it's flying
        titleDrones_[i].setCorners(pointAtAngle(drone, facing, 18),
                                   pointAtAngle(drone, facing + 2.5, 13),
                                   pointAtAngle(drone, facing - 2.5, 13));
    }
    titleSlime_.setPosition(Point(center.x - 48, center.y - 60 + std::sin(stateTimer_ * 3) * 8));
}

void Game::updatePlaying(double dt) {
    if (window_.wasKeyPressed('P') || window_.wasKeyPressed(Key::Escape)) {
        state_ = GameState::Paused;
        return;
    }

    survivalTime_ += dt;
    updateWaves();

    player_.update(window_, dt);

    spawnEnemies(dt);
    for (Enemy& enemy : enemies_) {
        enemy.update(dt, player_.getPosition());
    }
    keepEnemiesApart();

    if (player_.isReadyToFire()) {
        fireAtNearestEnemy();
    }
    for (Bullet& bullet : bullets_) {
        bullet.update(dt);
    }
    handleBulletHits();

    for (Coin& coin : coins_) {
        coin.update(dt, player_.getPosition(), player_.getMagnetRange());
    }

    updateEffects(dt);
    handlePlayerHits();   // may switch to the Dying state
    if (state_ == GameState::Playing) {
        collectCoins();   // may switch to the LevelUp state
    }
    removeFinishedObjects();
}

void Game::updatePaused() {
    if (window_.wasKeyPressed('P') || window_.wasKeyPressed(Key::Escape)) {
        state_ = GameState::Playing;
    } else if (window_.wasKeyPressed('Q')) {
        state_ = GameState::Title;
        stateTimer_ = 0;
    }
}

void Game::updateLevelUp(double dt) {
    stateTimer_ += dt;

    // The arena is frozen, but explosions and messages keep animating.
    updateEffects(dt);
    removeFinishedObjects();

    for (UpgradeCard& card : cards_) {
        card.update(window_, dt);
    }
    for (const UpgradeCard& card : cards_) {
        if (card.wasChosen(window_)) {
            Upgrade upgrade = card.getUpgrade();
            player_.applyUpgrade(upgrade);
            Color color = getUpgradeColor(upgrade);
            Point player = player_.getPosition();
            explode(player, color, 40, 300);
            popup(getUpgradeName(upgrade) + "!", player.x, player.y - 70, color, 2);
            cards_.clear();
            state_ = GameState::Playing;
            return;
        }
    }
}

void Game::updateDying(double dt) {
    stateTimer_ += dt;

    // Slow motion: everything moves at 30% speed.
    double slow = dt * 0.3;
    for (Enemy& enemy : enemies_) {
        enemy.update(slow, player_.getPosition());
    }
    keepEnemiesApart();
    for (Bullet& bullet : bullets_) {
        bullet.update(slow);
    }
    updateEffects(slow);
    removeFinishedObjects();

    if (stateTimer_ > 1.8) {
        state_ = GameState::GameOver;
        stateTimer_ = 0;
    }
}

void Game::updateGameOver(double dt) {
    stateTimer_ += dt;
    updateEffects(dt);
    removeFinishedObjects();

    // Wait a moment so a key held during the action doesn't skip this screen.
    if (stateTimer_ < 0.8) return;

    if (window_.wasKeyPressed(Key::Space) || window_.wasKeyPressed(Key::Enter) ||
        window_.wasMouseClicked()) {
        startNewGame();
    } else if (window_.wasKeyPressed(Key::Escape)) {
        state_ = GameState::Title;
        stateTimer_ = 0;
    }
}

// =============================================================================
//  Gameplay
// =============================================================================

void Game::startNewGame() {
    player_.reset();
    enemies_.clear();
    bullets_.clear();
    coins_.clear();
    particles_.clear();
    popups_.clear();
    cards_.clear();

    survivalTime_ = 0;
    wave_ = 1;
    spawnTimer_ = 1.0;
    kills_ = 0;
    level_ = 1;
    coinsCollected_ = 0;
    coinsNeeded_ = 5;
    newBest_ = false;
    shake_ = 0;
    flashStrength_ = 0;
    displayedHealth_ = player_.getHealth();

    state_ = GameState::Playing;
    stateTimer_ = 0;
    popup("WAVE 1", SCREEN_WIDTH / 2.0, 200, Color(255, 170, 40), 6);
}

// Starts a new wave every WAVE_LENGTH seconds, with a ring of drones.
void Game::updateWaves() {
    int currentWave = 1 + (int)(survivalTime_ / WAVE_LENGTH);
    if (currentWave > wave_) {
        wave_ = currentWave;
        popup("WAVE " + std::to_string(wave_), SCREEN_WIDTH / 2.0, 200, Color(255, 170, 40), 6);
        flashScreen(Color(255, 170, 40), 0.25);
        shakeScreen(8);
        spawnRingOfDrones();
    }
}

void Game::spawnEnemies(double dt) {
    spawnTimer_ -= dt;
    if (spawnTimer_ > 0 || (int)enemies_.size() >= MAX_ENEMIES) return;

    // Each wave spawns enemies 20% more often than the last.
    spawnTimer_ = 0.95 * std::pow(0.8, wave_ - 1);
    if (spawnTimer_ < 0.1) spawnTimer_ = 0.1;

    // Later waves bring new kinds of enemies.
    double roll = randomDouble(0, 1);
    EnemyType type = EnemyType::Drone;
    if (wave_ >= 3 && roll < 0.12) {
        type = EnemyType::Brute;
    } else if (wave_ >= 2 && roll < 0.35) {
        type = EnemyType::Darter;
    }
    spawnAtEdge(type);

    // From wave 4 on, drones sometimes arrive in packs.
    if (wave_ >= 4 && randomInt(0, 3) == 0) {
        spawnAtEdge(EnemyType::Drone);
        spawnAtEdge(EnemyType::Drone);
    }
}

void Game::spawnEnemy(EnemyType type, Point start) {
    enemies_.push_back(Enemy(type, start, nextEnemyId_));
    nextEnemyId_++;
}

// Spawns an enemy just outside a random edge of the screen.
void Game::spawnAtEdge(EnemyType type) {
    const double margin = 40;
    Point start;
    switch (randomInt(0, 3)) {
        case 0:  start = Point(randomDouble(0, SCREEN_WIDTH), -margin); break;                 // top
        case 1:  start = Point(randomDouble(0, SCREEN_WIDTH), SCREEN_HEIGHT + margin); break;  // bottom
        case 2:  start = Point(-margin, randomDouble(0, SCREEN_HEIGHT)); break;                // left
        default: start = Point(SCREEN_WIDTH + margin, randomDouble(0, SCREEN_HEIGHT)); break;  // right
    }
    spawnEnemy(type, start);
}

// A circle of drones closing in on the player from every side.
void Game::spawnRingOfDrones() {
    int count = 8 + wave_ * 2;
    for (int i = 0; i < count; i++) {
        double angle = i * 2 * PI / count;
        spawnEnemy(EnemyType::Drone, pointAtAngle(player_.getPosition(), angle, 460));
    }
}

void Game::fireAtNearestEnemy() {
    Point player = player_.getPosition();

    // Find the closest enemy within range.
    int nearest = -1;
    double nearestDistance = TARGET_RANGE;
    for (size_t i = 0; i < enemies_.size(); i++) {
        double d = player.distanceTo(enemies_[i].getPosition());
        if (d < nearestDistance) {
            nearestDistance = d;
            nearest = (int)i;
        }
    }
    if (nearest < 0) return;   // nothing to shoot at

    Point target = enemies_[nearest].getPosition();
    double aim = std::atan2(target.y - player.y, target.x - player.x);

    // Fan the bullets out evenly around the aim direction.
    int count = player_.getBulletCount();
    for (int i = 0; i < count; i++) {
        double angle = aim + (i - (count - 1) / 2.0) * BULLET_SPREAD;
        bullets_.push_back(Bullet(player, angle, BULLET_SPEED,
                                  player_.getDamage(), player_.getPierce()));
    }

    // Muzzle flash.
    Point muzzle = pointAtAngle(player, aim, 18);
    for (int i = 0; i < 4; i++) {
        double angle = aim + randomDouble(-0.6, 0.6);
        double speed = randomDouble(80, 220);
        particles_.push_back(Particle(muzzle, std::cos(angle) * speed, std::sin(angle) * speed,
                                      2.5, Color(255, 220, 100), 0.2));
    }
    player_.restartFireTimer();
}

void Game::handleBulletHits() {
    for (Bullet& bullet : bullets_) {
        for (Enemy& enemy : enemies_) {
            if (bullet.isFinished()) break;
            if (enemy.isDead() || bullet.hasHit(enemy.getId())) continue;

            double d = bullet.getPosition().distanceTo(enemy.getPosition());
            if (d < bullet.getRadius() + enemy.getRadius()) {
                bullet.recordHit(enemy.getId());

                // Knock the enemy back in the direction the bullet was flying.
                double speed = std::hypot(bullet.getVelocityX(), bullet.getVelocityY());
                enemy.hit(bullet.getDamage(),
                          bullet.getVelocityX() / speed * 260,
                          bullet.getVelocityY() / speed * 260);

                explode(bullet.getPosition(), Color::White, 4, 160);
            }
        }
    }

    // Now remove the enemies that died. Go backward so erasing doesn't skip any.
    for (int i = (int)enemies_.size() - 1; i >= 0; i--) {
        if (enemies_[i].isDead()) defeatEnemy(i);
    }
}

void Game::defeatEnemy(int index) {
    const Enemy& enemy = enemies_[index];
    Point where = enemy.getPosition();
    bool brute = enemy.getType() == EnemyType::Brute;

    explode(where, enemy.getColor(), brute ? 45 : 18, brute ? 340 : 240);
    explode(where, Color::White, brute ? 12 : 5, 180);
    shakeScreen(brute ? 12 : 3);
    if (brute) flashScreen(Color::White, 0.15);

    for (int i = 0; i < enemy.getCoinValue(); i++) {
        coins_.push_back(Coin(coinSheet_, where));
    }
    kills_++;
    enemies_.erase(enemies_.begin() + index);
}

void Game::handlePlayerHits() {
    Point player = player_.getPosition();

    for (const Enemy& enemy : enemies_) {
        double d = player.distanceTo(enemy.getPosition());
        if (d < enemy.getRadius() + player_.getRadius() * 0.8) {
            if (!player_.takeDamage()) return;   // still invulnerable

            shakeScreen(16);
            flashScreen(Color(255, 40, 60), 0.3);
            explode(player, Color(120, 230, 90), 24, 260);

            // A shockwave knocks nearby enemies away, giving the player room.
            for (Enemy& other : enemies_) {
                Point o = other.getPosition();
                double od = player.distanceTo(o);
                if (od < 180 && od > 0) {
                    other.push((o.x - player.x) / od * 700, (o.y - player.y) / od * 700);
                }
            }
            for (int i = 0; i < 36; i++) {
                double angle = i * 2 * PI / 36;
                particles_.push_back(Particle(player, std::cos(angle) * 520, std::sin(angle) * 520,
                                              3, Color(255, 255, 255, 200), 0.35));
            }

            if (player_.isDead()) startDying();
            return;
        }
    }
}

void Game::startDying() {
    state_ = GameState::Dying;
    stateTimer_ = 0;
    explode(player_.getPosition(), Color(120, 230, 90), 90, 480);
    explode(player_.getPosition(), Color::White, 30, 300);
    shakeScreen(26);
    flashScreen(Color::White, 0.7);

    if (survivalTime_ > bestTime_) {
        bestTime_ = survivalTime_;
        newBest_ = true;
    }
}

void Game::collectCoins() {
    for (int i = (int)coins_.size() - 1; i >= 0; i--) {
        Point coin = coins_[i].getPosition();
        if (player_.getPosition().distanceTo(coin) < 22) {
            explode(coin, Color(253, 224, 60), 5, 120);
            coins_.erase(coins_.begin() + i);
            coinsCollected_++;
        }
    }
    if (coinsCollected_ >= coinsNeeded_) {
        openLevelUp();
    }
}

void Game::openLevelUp() {
    coinsCollected_ -= coinsNeeded_;
    coinsNeeded_ += 4;   // each level needs a few more coins
    level_++;

    // Three different upgrades, side by side.
    std::vector<Upgrade> choices = pickRandomUpgrades(3);
    const double gap = 30;
    double totalWidth = 3 * UpgradeCard::WIDTH + 2 * gap;
    double left = (SCREEN_WIDTH - totalWidth) / 2;
    cards_.clear();
    for (int i = 0; i < 3; i++) {
        cards_.push_back(UpgradeCard(choices[i], left + i * (UpgradeCard::WIDTH + gap),
                                     200, i + 1, i * 0.08));
    }

    flashScreen(Color(253, 224, 60), 0.3);
    state_ = GameState::LevelUp;
    stateTimer_ = 0;
}

// Pushes apart any two enemies that overlap, so the swarm spreads out.
void Game::keepEnemiesApart() {
    for (size_t i = 0; i < enemies_.size(); i++) {
        for (size_t j = i + 1; j < enemies_.size(); j++) {
            Point a = enemies_[i].getPosition();
            Point b = enemies_[j].getPosition();
            double minDistance = enemies_[i].getRadius() + enemies_[j].getRadius();
            double dx = b.x - a.x, dy = b.y - a.y;
            if (std::abs(dx) >= minDistance || std::abs(dy) >= minDistance) continue;   // quick check

            double d = a.distanceTo(b);
            if (d >= minDistance || d == 0) continue;
            // Move each enemy half of the overlap, in opposite directions.
            double overlap = (minDistance - d) / 2;
            enemies_[i].nudge(-dx / d * overlap, -dy / d * overlap);
            enemies_[j].nudge(dx / d * overlap, dy / d * overlap);
        }
    }
}

// Particles, messages, and the fading of shake, flash, and the health bar.
void Game::updateEffects(double dt) {
    for (Particle& particle : particles_) {
        particle.update(dt);
    }
    for (FloatingText& text : popups_) {
        text.update(dt);
    }

    double fade = 1.0 - 8.0 * dt;
    if (fade < 0) fade = 0;
    shake_ *= fade;
    flashStrength_ -= 2.5 * dt;
    if (flashStrength_ < 0) flashStrength_ = 0;

    // The white "chip" on the health bar drains slowly toward the real health.
    if (displayedHealth_ > player_.getHealth()) {
        displayedHealth_ -= 1.5 * dt;
    }
    if (displayedHealth_ < player_.getHealth()) {
        displayedHealth_ = player_.getHealth();
    }
}

void Game::removeFinishedObjects() {
    for (int i = (int)bullets_.size() - 1; i >= 0; i--) {
        if (bullets_[i].isFinished()) bullets_.erase(bullets_.begin() + i);
    }
    for (int i = (int)particles_.size() - 1; i >= 0; i--) {
        if (!particles_[i].isAlive()) particles_.erase(particles_.begin() + i);
    }
    for (int i = (int)popups_.size() - 1; i >= 0; i--) {
        if (!popups_[i].isAlive()) popups_.erase(popups_.begin() + i);
    }
    // Too many particles slows things down; drop the oldest ones.
    if ((int)particles_.size() > MAX_PARTICLES) {
        particles_.erase(particles_.begin(), particles_.end() - MAX_PARTICLES);
    }
}

// =============================================================================
//  Special effects
// =============================================================================

// Sends `count` particles flying out from `center` in random directions.
void Game::explode(Point center, Color color, int count, double speed) {
    for (int i = 0; i < count; i++) {
        double angle = randomDouble(0, 2 * PI);
        double s = randomDouble(speed * 0.3, speed);
        particles_.push_back(Particle(center, std::cos(angle) * s, std::sin(angle) * s,
                                      randomDouble(2, 5), color, randomDouble(0.3, 0.8)));
    }
}

void Game::shakeScreen(double amount) {
    if (amount > shake_) shake_ = amount;
}

void Game::flashScreen(Color color, double strength) {
    flashColor_ = color;
    flashStrength_ = strength;
}

void Game::popup(const std::string& message, double centerX, double y, Color color, int scale) {
    popups_.push_back(FloatingText(message, centerX, y, color, scale, 1.6));
}

// =============================================================================
//  Drawing
// =============================================================================

void Game::draw() {
    window_.clear(GROUND);

    if (state_ == GameState::Title) {
        drawBackground();
        drawTitle();
    } else {
        drawWorld();
        drawHud();
        if (state_ == GameState::LevelUp)  drawLevelUp();
        if (state_ == GameState::Paused)   drawPaused();
        if (state_ == GameState::GameOver) drawGameOver();
    }

    if (flashStrength_ > 0 && state_ != GameState::Paused) {
        flash_.setColor(flashColor_.withAlpha((int)(255 * flashStrength_)));
        flash_.draw(window_);
    }
}

// The floor: specks and a slowly drifting grid.
void Game::drawBackground() {
    for (const Circle& pebble : pebbles_) {
        pebble.draw(window_);
    }

    // Slide the grid diagonally; after 4 squares it lines up again, so wrap.
    double offset = std::fmod(window_.getTime() * 12, GRID_SIZE * 4);
    int columns = SCREEN_WIDTH / GRID_SIZE + 6;
    for (int i = 0; i < (int)gridLines_.size(); i++) {
        double pos = (i < columns ? i : i - columns) * GRID_SIZE - GRID_SIZE * 4 + offset;
        if (i < columns) {   // vertical line
            gridLines_[i].setStart(Point(pos, -GRID_SIZE));
            gridLines_[i].setEnd(Point(pos, SCREEN_HEIGHT + GRID_SIZE));
        } else {             // horizontal line
            gridLines_[i].setStart(Point(-GRID_SIZE, pos));
            gridLines_[i].setEnd(Point(SCREEN_WIDTH + GRID_SIZE, pos));
        }
        gridLines_[i].draw(window_);
    }
}

// Everything in the arena. Shaking the screen = moving the camera randomly.
void Game::drawWorld() {
    double shakeX = randomDouble(-shake_, shake_);
    double shakeY = randomDouble(-shake_, shake_);
    window_.setCamera(shakeX, shakeY);

    drawBackground();
    for (const Coin& coin : coins_) {
        coin.draw(window_);
    }
    for (const Enemy& enemy : enemies_) {
        enemy.draw(window_);
    }
    for (const Bullet& bullet : bullets_) {
        bullet.draw(window_);
    }
    if (!player_.isDead()) {
        player_.draw(window_);
    }
    for (const Particle& particle : particles_) {
        particle.draw(window_);
    }
    for (const FloatingText& text : popups_) {
        text.draw(window_);
    }

    // The edges of the screen and the HUD don't shake.
    window_.setCamera(0, 0);
    for (const Rectangle& band : vignette_) {
        band.draw(window_);
    }
}

void Game::drawHud() {
    // Health bar.
    double maxHealth = player_.getMaxHealth();
    double fraction = player_.getHealth() / maxHealth;
    healthChip_.setSize(220 * displayedHealth_ / maxHealth, 18);
    healthFill_.setSize(220 * fraction, 18);
    if (fraction > 0.5) {
        healthFill_.setColor(Color(90, 230, 120));
    } else if (fraction > 0.25) {
        healthFill_.setColor(Color(253, 200, 60));
    } else {
        // Low health: pulse red.
        int pulse = (int)(60 * std::sin(window_.getTime() * 10));
        healthFill_.setColor(Color(230 + pulse / 3, 50, 60));
    }
    healthText_.setText("HP " + std::to_string(player_.getHealth()) + " / " +
                        std::to_string(player_.getMaxHealth()));
    healthBack_.draw(window_);
    healthChip_.draw(window_);
    healthFill_.draw(window_);
    healthText_.draw(window_);

    levelText_.setText("LV " + std::to_string(level_));
    levelText_.draw(window_);

    timeText_.setText(formatTime(survivalTime_));
    drawCentered(timeText_, 14);
    waveText_.setText("WAVE " + std::to_string(wave_));
    drawCentered(waveText_, 52);

    killsText_.setText("KILLS " + std::to_string(kills_));
    killsText_.setX(SCREEN_WIDTH - 20 - killsText_.getWidth());
    killsText_.draw(window_);

    // Coins toward the next level, along the bottom of the screen.
    xpFill_.setSize(SCREEN_WIDTH * (double)coinsCollected_ / coinsNeeded_, 8);
    xpBack_.draw(window_);
    xpFill_.draw(window_);
}

void Game::drawTitle() {
    double centerY = 390;

    // Drones behind the slime (the top half of the oval) are drawn first.
    for (const Triangle& drone : titleDrones_) {
        if (drone.getCorner1().y < centerY) drone.draw(window_);
    }
    titleSlime_.draw(window_);
    for (const Triangle& drone : titleDrones_) {
        if (drone.getCorner1().y >= centerY) drone.draw(window_);
    }

    for (const Rectangle& band : vignette_) {
        band.draw(window_);
    }

    drawCentered(titleSmall_, 60);
    // Glow between red and orange.
    int glow = (int)(50 + 50 * std::sin(stateTimer_ * 2));
    titleBig_.setColor(Color(255, 60 + glow, 80 - glow / 2));
    drawCentered(titleBig_, 100);

    int pulse = (int)(160 + 95 * std::sin(stateTimer_ * 4));
    titlePrompt_.setColor(Color::White.withAlpha(pulse));
    drawCentered(titlePrompt_, 540);
    drawCentered(titleHelp_, 580);
    if (bestTime_ > 0) {
        titleBest_.setText("Best time: " + formatTime(bestTime_));
        drawCentered(titleBest_, 605);
    }
}

void Game::drawLevelUp() {
    dim_.draw(window_);
    drawCentered(levelUpTitle_, 80 + std::sin(stateTimer_ * 4) * 6);
    drawCentered(levelUpHint_, 150);
    for (const UpgradeCard& card : cards_) {
        card.draw(window_);
    }
}

void Game::drawPaused() {
    dim_.draw(window_);
    drawCentered(pausedTitle_, 250);
    drawCentered(pausedHelp_, 350);
}

void Game::drawGameOver() {
    dim_.draw(window_);
    drawCentered(gameOverTitle_, 170);

    gameOverStats_.setText("Survived " + formatTime(survivalTime_) +
                           "    Defeated " + std::to_string(kills_) +
                           "    Level " + std::to_string(level_));
    drawCentered(gameOverStats_, 280);

    if (newBest_) {
        // Blink between yellow and white.
        bool white = (int)(stateTimer_ * 4) % 2 == 0;
        gameOverBest_.setText("NEW BEST TIME!");
        gameOverBest_.setColor(white ? Color::White : Color(253, 224, 60));
    } else {
        gameOverBest_.setText("Best time: " + formatTime(bestTime_));
        gameOverBest_.setColor(Color(253, 224, 60));
    }
    drawCentered(gameOverBest_, 320);

    if (stateTimer_ > 0.8) {
        drawCentered(gameOverPrompt_, 420);
        drawCentered(gameOverHelp_, 460);
    }
}

// Draws text centered across the screen with its top at y, plus a shadow.
void Game::drawCentered(Text& text, double y) {
    Color color = text.getColor();
    int offset = text.getScale();   // bigger text gets a bigger shadow
    double left = (SCREEN_WIDTH - text.getWidth()) / 2;

    text.setPosition(Point(left + offset, y + offset));
    text.setColor(Color(0, 0, 0, color.a * 2 / 3));
    text.draw(window_);

    text.setPosition(Point(left, y));
    text.setColor(color);
    text.draw(window_);
}
