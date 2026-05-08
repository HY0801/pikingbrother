#ifndef CONSTANTS_H
#define CONSTANTS_H

enum GameState {
    MENU,
    GUIDE,
    LEVEL_SELECT,
    PLAYING,
    LEVEL_COMPLETE,
    GAME_OVER,
    VICTORY
};

enum CollectibleType {
    XIONGDA,
    XIONGER,
    GUANGTOUQIANG
};

const float GRAVITY = 0.7f;
const float JUMP_VEL = -13.0f;
const float PLAYER_WIDTH = 60.0f;
const float PLAYER_HEIGHT = 80.0f;
const float COLLECT_SIZE = 50.0f;

#endif // CONSTANTS_H