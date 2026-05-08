#include "gameworld.h"
#include "constants.h"
#include <cmath>

GameWorld::GameWorld()
    : lives(3), currentLevel(1), mapOffset(0), gameOverFlag(false), victoryFlag(false),
    aPressed(false), dPressed(false), leftPressed(false), rightPressed(false),
    wPressed(false), upPressed(false), qPressed(false), kPressed(false),
    attackCooldown1(0), attackCooldown2(0)
{
    init(1);
}

void GameWorld::init(int level) {
    lives = 3;
    currentLevel = level;
    gameOverFlag = false;
    victoryFlag = false;
    mapOffset = 0;
    attackCooldown1 = attackCooldown2 = 0;
    loadLevel(currentLevel);
    resetPositions();
}

void GameWorld::loadLevel(int level) {
    platforms.clear();
    obstacles.clear();
    zombies.clear();
    collectibles.clear();

    // 地面（无限长） - 注意：y=520, 高80，玩家脚底y+80=520
    platforms.push_back({0, 520, 5000, 80});

    // 房子放在起始点（左侧）
    houseX = 200;
    houseY = 440;

    // 通用高台（所有关卡都有）
    platforms.push_back({500, 460, 120, 20});
    platforms.push_back({800, 400, 120, 20});
    platforms.push_back({1200, 330, 120, 20});
    if(level == 3) {
        platforms.push_back({1500, 250, 120, 20});
    }

    // 可拾取物
    collectibles.push_back({600, 440, XIONGDA, true});
    collectibles.push_back({1050, 440, XIONGER, true});
    if(level == 1) {
        collectibles.push_back({1500, 370, GUANGTOUQIANG, true});
    } else if(level == 2) {
        collectibles.push_back({1450, 310, GUANGTOUQIANG, true});
    } else if(level == 3) {
        collectibles.push_back({1600, 230, GUANGTOUQIANG, true});
    }

    // 障碍物
    obstacles.push_back({1000, 480, 60, 40});
    obstacles.push_back({1800, 480, 60, 40});

    // 僵尸
    if(level >= 2) {
        zombies.push_back({700, 440, -2, true});
        zombies.push_back({1300, 440, 2, true});
    }
    if(level >= 3) {
        zombies.push_back({850, 440, -2, true});
        zombies.push_back({1550, 440, 2, true});
        zombies.push_back({2000, 440, -1, true});
        zombies.push_back({2300, 440, 1, true});
    }
}

void GameWorld::resetPositions() {
    p1.reset(150, 440);
    p2.reset(350, 440);
}

void GameWorld::setInput(bool a, bool d, bool left, bool right, bool w, bool up, bool q, bool k) {
    aPressed = a; dPressed = d;
    leftPressed = left; rightPressed = right;
    wPressed = w; upPressed = up;
    qPressed = q; kPressed = k;
}

void GameWorld::update() {
    if(gameOverFlag || victoryFlag) return;

    float moveSpeed = 6;
    // 玩家1移动 + 摄像机
    if(aPressed) {
        if(mapOffset > 0 && p1.x <= 350) mapOffset -= moveSpeed;
        else p1.move(-moveSpeed);
    }
    if(dPressed) {
        if(p1.x < 350) p1.move(moveSpeed);
        else mapOffset += moveSpeed;
    }
    if(leftPressed) p2.move(-moveSpeed);
    if(rightPressed) p2.move(moveSpeed);

    if(mapOffset < 0) mapOffset = 0;
    if(mapOffset > 4000) mapOffset = 4000;

    // 跳跃
    if(wPressed) p1.jump();
    if(upPressed) p2.jump();

    // 攻击冷却
    if(attackCooldown1 > 0) attackCooldown1--;
    if(attackCooldown2 > 0) attackCooldown2--;
    if(qPressed && attackCooldown1 == 0) {
        attackCooldown1 = 15;
        QRectF attackRect(p1.x-40, p1.y, 140, 80);
        for(auto& z : zombies) {
            if(z.alive && attackRect.intersects(z.rect()))
                z.alive = false;
        }
    }
    if(kPressed && attackCooldown2 == 0) {
        attackCooldown2 = 15;
        QRectF attackRect(p2.x-40, p2.y, 140, 80);
        for(auto& z : zombies) {
            if(z.alive && attackRect.intersects(z.rect()))
                z.alive = false;
        }
    }

    // 物理更新
    applyGravityAndPlatform(p1);
    applyGravityAndPlatform(p2);
    updateZombies();
    checkCollectAndDelivery();
    checkDeathAndRespawn();
    checkLevelComplete();
}

void GameWorld::applyGravityAndPlatform(Player &p) {
    // 先应用重力
    p.applyGravity();

    // 先强制地面碰撞（确保站在地面 y=520）
    if(p.y + PLAYER_HEIGHT >= 520) {
        p.y = 520 - PLAYER_HEIGHT;
        p.vy = 0;
        p.onGround = true;
    }

    // 再处理其他平台（高台）
    QRectF playerRect = p.rect();
    for(auto& plat : platforms) {
        // 跳过地面平台（因为已经用强制地面处理了，避免重复）
        if(plat.y >= 520) continue;

        QRectF platRect = plat.rect();
        if(playerRect.intersects(platRect)) {
            // 从上往下落
            if(p.vy >= 0 && (playerRect.bottom() - p.vy) <= platRect.top()) {
                p.setPosition(p.x, platRect.top() - PLAYER_HEIGHT);
                p.vy = 0;
                p.onGround = true;
            }
            // 从下往上顶头
            else if(p.vy < 0 && playerRect.top() >= platRect.bottom()) {
                p.setPosition(p.x, platRect.bottom());
                p.vy = 0;
            }
        }
    }

    // 坠落死亡（y > 620）
    if(p.y > 620) {
        lives--;
        resetPositions();
        if(lives <= 0) gameOverFlag = true;
    }
}

void GameWorld::updateZombies() {
    for(auto& z : zombies) {
        if(!z.alive) continue;
        z.x += z.vx;
        if(z.x < 200 || z.x > 3800) z.vx = -z.vx;
        QRectF zRect = z.rect();
        if(zRect.intersects(p1.rect()) || zRect.intersects(p2.rect())) {
            lives--;
            resetPositions();
            if(lives <= 0) gameOverFlag = true;
            return;
        }
    }
}

void GameWorld::checkCollectAndDelivery() {
    // 拾取
    for(auto& c : collectibles) {
        if(!c.exists) continue;
        QRectF collectRect = c.rect();
        if(collectRect.intersects(p1.rect()) && p1.carryCount == 0) {
            c.exists = false;
            p1.carryCount = 1;
            if(c.type == XIONGDA) p1.carryXiongDa = true;
            else if(c.type == XIONGER) p1.carryXiongEr = true;
            else p1.carryGuang = true;
        }
        else if(collectRect.intersects(p2.rect()) && p2.carryCount == 0) {
            c.exists = false;
            p2.carryCount = 1;
            if(c.type == XIONGDA) p2.carryXiongDa = true;
            else if(c.type == XIONGER) p2.carryXiongEr = true;
            else p2.carryGuang = true;
        }
    }

    // 交付
    QRectF houseRect(houseX, houseY, 150, 150);
    if(houseRect.intersects(p1.rect()) && p1.carryCount > 0) {
        p1.carryCount = 0;
        p1.carryXiongDa = p1.carryXiongEr = p1.carryGuang = false;
    }
    if(houseRect.intersects(p2.rect()) && p2.carryCount > 0) {
        p2.carryCount = 0;
        p2.carryXiongDa = p2.carryXiongEr = p2.carryGuang = false;
    }
}

void GameWorld::checkDeathAndRespawn() {
    QRectF p1r = p1.rect(), p2r = p2.rect();
    for(auto& obs : obstacles) {
        if(p1r.intersects(obs.rect()) || p2r.intersects(obs.rect())) {
            lives--;
            resetPositions();
            if(lives <= 0) gameOverFlag = true;
            return;
        }
    }
}

void GameWorld::checkLevelComplete() {
    bool allCollected = true;
    for(auto& c : collectibles) {
        if(c.exists) { allCollected = false; break; }
    }
    if(allCollected && p1.carryCount == 0 && p2.carryCount == 0) {
        if(currentLevel < 3) {
            currentLevel++;
            loadLevel(currentLevel);
            resetPositions();
            mapOffset = 0;
        } else {
            victoryFlag = true;
        }
    }
}

void GameWorld::resetToMenu() {
    gameOverFlag = false;
    victoryFlag = false;
    init(currentLevel);
}