#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QPushButton>
#include <QPixmap>
#include <QSoundEffect>
#include <QMediaPlayer>
#include <QAudioOutput>
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
    void onStartPrelude();

private:
    GameState gameState;
    GameWorld gameWorld;

    QTimer *timer;
    QPushButton *startButton, *guideButton, *exitButton, *backButton;
    QPushButton *level1Btn, *level2Btn, *level3Btn;
    QPushButton *nextLevelButton, *exitAfterLevelButton;
    QPushButton *preludeStartButton;

    QPixmap bg1, bg2, bg3;
    QPixmap piggy, lulu;
    QPixmap piggySuper, luluSuper;
    QPixmap xiongda, xionger, guangtouqiang;
    QPixmap tutu, xiaomei, xiaoguai, shuazi;
    QPixmap xiyangyang, meiyangyang, nuanyangyang, feiyangyang, lanyangyang;
    QPixmap housePic, house2Pic, house3Pic;
    QPixmap zombiePic, tutumaPic, huitailangPic;
    QPixmap obstaclePic, startCover, guidePage, levelMapPic, fartEffectPic, attackEffectPic, heartPic;
    QPixmap failBg, endBg;
    QPixmap preludePic1, preludePic2, preludePic3;

    int fartTimer1, fartTimer2;
    QPointF fartPos1, fartPos2;

    int pendingLevel;

    QSoundEffect *fartSound;
    QSoundEffect *attackSound;
    QMediaPlayer *bgmPlayer;
    QAudioOutput *bgmOutput;

    bool aPressed, dPressed, leftPressed, rightPressed, wPressed, upPressed, qPressed, kPressed;

    void initUI();
    void loadImages();
    void drawGame(QPainter &painter);
    void showPrelude(int level);
    void initAudio();
    void playBGMForState(int forceLevel = -1);
    void resetKeyStates();
};

#endif // MAINWINDOW_H