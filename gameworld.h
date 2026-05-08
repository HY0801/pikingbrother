#ifndef GAMEWORLD_H
#define GAMEWORLD_H

#include <QVector>
#include "player.h"

struct Platform {
    float x, y, w, h;
    QRectF rect() const { return QRectF(x, y, w, h); }
};

struct Obstacle {
    float x, y, w, h;
    QRectF rect() const { return QRectF(x, y, w, h); }
};

struct Zombie {
    float x, y, vx;
    bool alive;
    QRectF rect() const { return QRectF(x, y, 50, 70); }
};

struct Collectible {
    float x, y;
    int type;
    bool exists;
    QRectF rect() const { return QRectF(x, y, 50, 60); }
};

class GameWorld {
public:
    GameWorld();

    void init(int level);
    void update();
    void setInput(bool a, bool d, bool left, bool right, bool wJump, bool upJump, bool qAttack, bool kAttack);

    // 访问器
    Player& getPlayer1() { return p1; }
    Player& getPlayer2() { return p2; }
    const QVector<Platform>& getPlatforms() const { return platforms; }
    const QVector<Obstacle>& getObstacles() const { return obstacles; }
    const QVector<Zombie>& getZombies() const { return zombies; }
    const QVector<Collectible>& getCollectibles() const { return collectibles; }
    float getHouseX() const { return houseX; }
    float getHouseY() const { return houseY; }
    float getMapOffset() const { return mapOffset; }
    int getLives() const { return lives; }
    int getCurrentLevel() const { return currentLevel; }
    bool isGameOver() const { return gameOverFlag; }
    bool isVictory() const { return victoryFlag; }

    void resetToMenu();

private:
    Player p1, p2;
    int lives;
    int currentLevel;
    float mapOffset;
    bool gameOverFlag;
    bool victoryFlag;

    QVector<Platform> platforms;
    QVector<Obstacle> obstacles;
    QVector<Zombie> zombies;
    QVector<Collectible> collectibles;
    float houseX, houseY;

    bool aPressed, dPressed;
    bool leftPressed, rightPressed;
    bool wPressed, upPressed;
    bool qPressed, kPressed;
    int attackCooldown1, attackCooldown2;

    void loadLevel(int level);
    void applyGravityAndPlatform(Player &p);
    void updateZombies();
    void checkCollectAndDelivery();
    void checkDeathAndRespawn();
    void checkLevelComplete();
    void resetPositions();
};

#endif // GAMEWORLD_H