#ifndef PLAYER_H
#define PLAYER_H

#include <QRectF>

class Player {
public:
    Player();
    void reset(float startX, float startY);

    float x, y;
    float vy;
    bool onGround;

    bool carryXiongDa;
    bool carryXiongEr;
    bool carryGuang;
    int carryCount;

    QRectF rect() const { return QRectF(x, y, 60, 80); }
    void applyGravity();
    void move(float dx);
    void jump();
    void setPosition(float nx, float ny);
};

#endif // PLAYER_H