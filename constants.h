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

// 第一关角色类型
enum CollectibleType {
    XIONGDA,
    XIONGER,
    GUANGTOUQIANG,
    // 第二关角色
    TUTU,
    XIAOMEI,
    XIAOGUAI,
    SHUAZI,
    // 第三关角色
    XIYANGYANG,
    MEIYANGYANG,
    NUANYANGYANG,
    FEIYANGYANG,
    LANYANGYANG
};

const float GRAVITY = 0.7f;
const float JUMP_VEL = -13.0f;
const float PLAYER_WIDTH = 60.0f;
const float PLAYER_HEIGHT = 80.0f;
const float COLLECT_SIZE = 50.0f;

#endif // CONSTANTS_H