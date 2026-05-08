#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QPushButton>
#include <QPixmap>
#include "gameworld.h"
#include "constants.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void paintEvent(QPaintEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

private slots:
    void updateGame();
    void onLevelSelected(int level);
    void onNextLevel();
    void onExitToMenu();

private:
    GameState gameState;
    GameWorld gameWorld;

    QTimer *timer;
    QPushButton *startButton, *guideButton, *exitButton, *backButton;
    QPushButton *level1Btn, *level2Btn, *level3Btn;
    QPushButton *nextLevelButton, *exitAfterLevelButton;

    // 背景
    QPixmap bg1, bg2, bg3;
    // 玩家
    QPixmap piggy, lulu;
    // 第一关角色
    QPixmap xiongda, xionger, guangtouqiang;
    // 第二关角色
    QPixmap tutu, xiaomei, xiaoguai, shuazi;
    // 第三关角色
    QPixmap xiyangyang, meiyangyang, nuanyangyang, feiyangyang, lanyangyang;
    // 房子
    QPixmap housePic, house2Pic, house3Pic;
    // 敌人
    QPixmap zombiePic, tutumaPic, huitailangPic;
    // 其他
    QPixmap obstaclePic, startCover, guidePage, levelMapPic, fartEffectPic, heartPic;

    int fartTimer1, fartTimer2;
    QPointF fartPos1, fartPos2;

    void initUI();
    void loadImages();
    void drawGame(QPainter &painter);
};

#endif // MAINWINDOW_H