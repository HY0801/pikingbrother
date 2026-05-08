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

private:
    GameState gameState;
    GameWorld gameWorld;

    QTimer *timer;
    QPushButton *startButton, *guideButton, *exitButton, *backButton;
    QPushButton *level1Btn, *level2Btn, *level3Btn;

    // 图片资源（直接加载外部文件）
    QPixmap background, piggy, lulu, xiongda, xionger, guangtouqiang;
    QPixmap housePic, zombiePic, obstaclePic, startCover, guidePage, levelMapPic;
    QPixmap fartEffectPic, heartPic;

    int fartTimer1, fartTimer2;
    QPointF fartPos1, fartPos2;

    void initUI();
    void loadImages();
    void drawGame(QPainter &painter);
    void drawPlayerWithCarry(QPainter &painter, const Player &p, const QPixmap &basePic);
};

#endif // MAINWINDOW_H