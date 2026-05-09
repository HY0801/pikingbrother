#include "gameworld.h"
#include "constants.h"
#include <cmath>
#include <QDebug>

GameWorld::GameWorld()
    : lives(3), currentLevel(1), mapOffset(0), gameOverFlag(false), victoryFlag(false),
    levelCompleteFlag(false),
    aPressed(false), dPressed(false), leftPressed(false), rightPressed(false),
    wPressed(false), upPressed(false), qPressed(false), kPressed(false),
    attackCooldown1(0), attackCooldown2(0), attackEffectTimer1(0), attackEffectTimer2(0), invincibleFrames(0)
{
    loadLevel(1);
    resetPositions();
    savedList.clear();
}

void GameWorld::init(int level) {
    lives = 3;
    currentLevel = level;
    gameOverFlag = false;
    victoryFlag = false;
    levelCompleteFlag = false;
    mapOffset = 0;
    attackCooldown1 = attackCooldown2 = 0;
    attackEffectTimer1 = attackEffectTimer2 = 0;
    savedList.clear();
    invincibleFrames = 30;
    loadLevel(currentLevel);
    resetPositions();
    if (p1.y + PLAYER_HEIGHT > 520) p1.y = 520 - PLAYER_HEIGHT;
    if (p2.y + PLAYER_HEIGHT > 520) p2.y = 520 - PLAYER_HEIGHT;
    p1.vy = 0; p1.onGround = true;
    p2.vy = 0; p2.onGround = true;
}

void GameWorld::resetPositions() {
    p1.reset(150, 440);
    p2.reset(350, 440);
}

void GameWorld::loadLevel(int level) {
    platforms.clear();
    obstacles.clear();
    zombies.clear();
    collectibles.clear();

    platforms.push_back({0, 520, 5000, 80});
    houseX = 200;
    houseY = 440;

    if (level == 1) {
        obstacles.push_back({800, 480, 60, 60});
        obstacles.push_back({1500, 480, 60, 60});

        platforms.push_back({2300, 460, 120, 20});
        platforms.push_back({2500, 420, 120, 20});
        collectibles.push_back({2560, 350, XIONGDA, true});

        platforms.push_back({2900, 460, 120, 20});
        platforms.push_back({3100, 400, 120, 20});
        collectibles.push_back({3160, 330, XIONGER, true});

        platforms.push_back({3500, 460, 120, 20});
        platforms.push_back({3700, 380, 120, 20});
        platforms.push_back({3900, 300, 120, 20});
        collectibles.push_back({3960, 230, GUANGTOUQIANG, true});

        zombies.push_back({1200, 440, -2, true});
        zombies.push_back({1800, 440, 2, true});
    }
    else if (level == 2) {
        obstacles.push_back({700, 480, 60, 60});
        obstacles.push_back({1200, 480, 60, 60});
        obstacles.push_back({1800, 480, 60, 60});

        platforms.push_back({2300, 460, 120, 20});
        collectibles.push_back({2360, 390, TUTU, true});
        platforms.push_back({2430, 400, 120, 20});
        collectibles.push_back({2490, 330, XIAOMEI, true});
        platforms.push_back({2560, 340, 120, 20});
        collectibles.push_back({2620, 270, XIAOGUAI, true});
        platforms.push_back({2690, 280, 120, 20});
        collectibles.push_back({2750, 210, SHUAZI, true});

        zombies.push_back({1100, 440, -2, true});
        zombies.push_back({1600, 440, 2, true});
        zombies.push_back({2100, 440, -1, true});
        zombies.push_back({2800, 440, 1, true});
    }
    else if (level == 3) {
        obstacles.push_back({600, 480, 60, 60});
        obstacles.push_back({1100, 480, 60, 60});
        obstacles.push_back({1600, 480, 60, 60});
        obstacles.push_back({2100, 480, 60, 60});

        platforms.push_back({2500, 460, 120, 20});
        collectibles.push_back({2560, 390, XIYANGYANG, true});
        platforms.push_back({2630, 400, 120, 20});
        collectibles.push_back({2690, 330, MEIYANGYANG, true});
        platforms.push_back({2760, 340, 120, 20});
        collectibles.push_back({2820, 270, NUANYANGYANG, true});
        platforms.push_back({2890, 280, 120, 20});
        collectibles.push_back({2950, 210, FEIYANGYANG, true});
        platforms.push_back({3020, 220, 120, 20});
        collectibles.push_back({3080, 150, LANYANGYANG, true});

        zombies.push_back({900, 440, -2, true});
        zombies.push_back({1300, 440, 2, true});
        zombies.push_back({1800, 440, -1, true});
        zombies.push_back({2300, 440, 1, true});
        zombies.push_back({2900, 440, -2, true});
        zombies.push_back({3300, 440, 2, true});
    }
}

void GameWorld::setInput(bool a, bool d, bool left, bool right, bool w, bool up, bool q, bool k) {
    aPressed = a; dPressed = d;
    leftPressed = left; rightPressed = right;
    wPressed = w; upPressed = up;
    qPressed = q; kPressed = k;
}

void GameWorld::update() {
    if(gameOverFlag || victoryFlag || levelCompleteFlag) return;
    if (invincibleFrames > 0) invincibleFrames--;

    float moveSpeed = 6;
    if(aPressed) p1.move(-moveSpeed);
    if(dPressed) p1.move(moveSpeed);
    if(leftPressed) p2.move(-moveSpeed);
    if(rightPressed) p2.move(moveSpeed);

    float centerX = (p1.x + p2.x) / 2.0f;
    float targetOffset = centerX - 400;
    if(targetOffset < 0) targetOffset = 0;
    if(targetOffset > 5000 - 800) targetOffset = 5000 - 800;
    mapOffset = targetOffset;

    if(p1.x < 0) p1.x = 0;
    if(p1.x > 5000 - 60) p1.x = 5000 - 60;
    if(p2.x < 0) p2.x = 0;
    if(p2.x > 5000 - 60) p2.x = 5000 - 60;

    if(wPressed) p1.jump();
    if(upPressed) p2.jump();

    if(attackCooldown1 > 0) attackCooldown1--;
    if(attackCooldown2 > 0) attackCooldown2--;

    if(qPressed && attackCooldown1 == 0) {
        attackCooldown1 = 15;
        attackEffectTimer1 = 20;   // 改为20，延长变身时间
        QRectF attackRect(p1.x-40, p1.y, 140, 80);
        for(auto& z : zombies) {
            if(z.alive && attackRect.intersects(z.rect()))
                z.alive = false;
        }
    }
    if(kPressed && attackCooldown2 == 0) {
        attackCooldown2 = 15;
        attackEffectTimer2 = 20;   // 改为20
        QRectF attackRect(p2.x-40, p2.y, 140, 80);
        for(auto& z : zombies) {
            if(z.alive && attackRect.intersects(z.rect()))
                z.alive = false;
        }
    }

    applyGravityAndPlatform(p1);
    applyGravityAndPlatform(p2);
    updateZombies();
    checkCollectAndDelivery();
    checkDeathAndRespawn();
    checkLevelComplete();

    if(attackEffectTimer1 > 0) attackEffectTimer1--;
    if(attackEffectTimer2 > 0) attackEffectTimer2--;
}

void GameWorld::applyGravityAndPlatform(Player &p) {
    float oldY = p.y;
    p.vy += GRAVITY;
    p.y += p.vy;

    bool grounded = false;
    QRectF playerRect = p.rect();

    for (auto& plat : platforms) {
        QRectF platRect = plat.rect();
        if (playerRect.intersects(platRect)) {
            float oldBottom = oldY + PLAYER_HEIGHT;
            if (oldBottom <= platRect.top() && p.vy >= 0) {
                p.y = platRect.top() - PLAYER_HEIGHT;
                p.vy = 0;
                grounded = true;
                break;
            }
            else if (p.vy < 0 && oldY >= platRect.bottom()) {
                p.y = platRect.bottom();
                p.vy = 0;
            }
        }
    }

    if (!grounded && p.y + PLAYER_HEIGHT >= 520) {
        p.y = 520 - PLAYER_HEIGHT;
        p.vy = 0;
        grounded = true;
    }

    p.onGround = grounded;

    if (p.y > 650 && p.x > 200) {
        if (invincibleFrames == 0 && lives > 0) lives--;
        resetPositions();
        p1.carryCount = 0; p1.carryType = -1;
        p2.carryCount = 0; p2.carryType = -1;
        if (lives <= 0) gameOverFlag = true;
        else {
            p.setPosition(p.x, 440);
            p.vy = 0;
            p.onGround = true;
        }
    }
}

void GameWorld::updateZombies() {
    for(auto& z : zombies) {
        if(!z.alive) continue;
        z.x += z.vx;
        if(z.x < 200 || z.x > 4800) z.vx = -z.vx;
        QRectF zRect = z.rect();
        if(zRect.intersects(p1.rect()) || zRect.intersects(p2.rect())) {
            if (invincibleFrames == 0 && lives > 0) {
                lives--;
                invincibleFrames = 15;
            }
            if (lives <= 0) gameOverFlag = true;
            return;
        }
    }
}

void GameWorld::checkCollectAndDelivery() {
    for(auto& c : collectibles) {
        if(!c.exists) continue;
        QRectF collectRect = c.rect();
        if(collectRect.intersects(p1.rect()) && p1.carryCount == 0) {
            c.exists = false;
            p1.carryCount = 1;
            p1.carryType = c.type;
        }
        else if(collectRect.intersects(p2.rect()) && p2.carryCount == 0) {
            c.exists = false;
            p2.carryCount = 1;
            p2.carryType = c.type;
        }
    }

    QRectF houseRect(houseX, houseY, 150, 150);
    if(houseRect.intersects(p1.rect()) && p1.carryCount > 0) {
        if (!savedList.contains(p1.carryType)) {
            savedList.append(p1.carryType);
        }
        p1.carryCount = 0;
        p1.carryType = -1;
    }
    if(houseRect.intersects(p2.rect()) && p2.carryCount > 0) {
        if (!savedList.contains(p2.carryType)) {
            savedList.append(p2.carryType);
        }
        p2.carryCount = 0;
        p2.carryType = -1;
    }
}

void GameWorld::checkDeathAndRespawn() {
    if (invincibleFrames > 0) return;

    QRectF p1r = p1.rect(), p2r = p2.rect();
    for(auto& obs : obstacles) {
        QPointF center(obs.x + obs.w/2, obs.y + obs.h/2);
        if(p1r.contains(center) || p2r.contains(center)) {
            if (lives > 0) lives--;
            invincibleFrames = 15;
            if (lives <= 0) gameOverFlag = true;
            return;
        }
    }
}

void GameWorld::checkLevelComplete() {
    bool allCollected = true;
    for(auto& c : collectibles) {
        if(c.exists) { allCollected = false; break; }
    }
    int required = 0;
    if (currentLevel == 1) required = 3;
    else if (currentLevel == 2) required = 4;
    else required = 5;
    if(allCollected && savedList.size() == required) {
        if(currentLevel < 3) {
            levelCompleteFlag = true;
        } else {
            victoryFlag = true;
        }
    }
}

void GameWorld::resetToMenu() {
    lives = 3;
    currentLevel = 1;
    gameOverFlag = false;
    victoryFlag = false;
    levelCompleteFlag = false;
    mapOffset = 0;
    attackCooldown1 = attackCooldown2 = 0;
    attackEffectTimer1 = attackEffectTimer2 = 0;
    savedList.clear();
    invincibleFrames = 30;
    loadLevel(currentLevel);
    resetPositions();
}

void GameWorld::nextLevel() {
    if(currentLevel < 3 && levelCompleteFlag) {
        currentLevel++;
        levelCompleteFlag = false;
        lives = 3;
        savedList.clear();
        invincibleFrames = 30;
        loadLevel(currentLevel);
        resetPositions();
        mapOffset = 0;
    }
}