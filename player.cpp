#include "player.h"
#include "constants.h"

Player::Player() : x(0), y(0), vy(0), onGround(true), carryCount(0), carryType(-1) {}

void Player::reset(float startX, float startY) {
    x = startX;
    y = startY;
    vy = 0;
    onGround = true;
    carryCount = 0;
    carryType = -1;
}

void Player::applyGravity() {
    vy += GRAVITY;
    y += vy;
}

void Player::move(float dx) {
    x += dx;
}

void Player::jump() {
    if(onGround) {
        vy = JUMP_VEL;
        onGround = false;
    }
}

void Player::setPosition(float nx, float ny) {
    x = nx;
    y = ny;
}