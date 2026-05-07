#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QPushButton>
#include <QPixmap>
#include <QPainter>
#include <QKeyEvent>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

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

private:

    // =========================
    // 游戏状态
    // =========================

    enum GameState
    {
        MENU,
        GUIDE,
        PLAYING
    };

    GameState gameState;

    // =========================
    // 定时器
    // =========================

    QTimer *timer;

    // =========================
    // 按钮
    // =========================

    QPushButton *startButton;

    QPushButton *guideButton;

    QPushButton *exitButton;

    QPushButton *backButton;

    // =========================
    // 玩家1
    // =========================

    int p1x;
    int p1y;
    int p1vy;

    // =========================
    // 玩家2
    // =========================

    int p2x;
    int p2y;
    int p2vy;

    // =========================
    // 按键
    // =========================

    bool aPressed;
    bool dPressed;

    bool leftPressed;
    bool rightPressed;

    // =========================
    // 地图偏移
    // =========================

    int mapOffset;

    // =========================
    // 游戏数据
    // =========================

    int lives;

    int score;

    // =========================
    // 图片资源
    // =========================

    QPixmap background;

    QPixmap piggy;

    QPixmap lulu;

    QPixmap startCover;

    QPixmap xiongda;

    QPixmap xionger;

    QPixmap guangtouqiang;

    QPixmap house;

    // =========================
    // 熊大
    // =========================

    int xdX;
    int xdY;

    // =========================
    // 熊二
    // =========================

    int xeX;
    int xeY;

    // =========================
    // 光头强
    // =========================

    int gtqX;
    int gtqY;

    // =========================
    // 房子
    // =========================

    int houseX;
    int houseY;

    // =========================
    // 当前携带角色
    // 0 无
    // 1 熊大
    // 2 熊二
    // 3 光头强
    // =========================

    int carryingType;

    // =========================
    // 谁在背
    // 0 无
    // 1 玩家1
    // 2 玩家2
    // =========================

    int carrierPlayer;

    // =========================
    // 是否已送回家
    // =========================

    bool xdSaved;

    bool xeSaved;

    bool gtqSaved;

    // =========================
    // 初始化游戏
    // =========================

    void initGame();
};

#endif // MAINWINDOW_H