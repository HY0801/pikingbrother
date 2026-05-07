#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    resize(800,600);
    setFixedSize(800,600);



    gameState = MENU;

    // =========================
    // 加载图片
    // =========================

    background = QPixmap("res/background.png");

    piggy = QPixmap("res/piggy.png");

    lulu = QPixmap("res/lulu.png");

    startCover = QPixmap("res/start_btn.png");

    xiongda = QPixmap("res/xiongda.png");

    xionger = QPixmap("res/xionger.png");

    guangtouqiang = QPixmap("res/guangtouqiang.png");

    // 光头强房子
    house = QPixmap("res/house.png");

    // =========================
    // 按钮
    // =========================

    startButton = new QPushButton("开始游戏",this);

    guideButton = new QPushButton("游戏指南",this);

    exitButton = new QPushButton("退出游戏",this);

    backButton = new QPushButton("返回",this);

    startButton->setGeometry(250,220,300,70);

    guideButton->setGeometry(250,320,300,70);

    exitButton->setGeometry(250,420,300,70);

    backButton->setGeometry(20,20,100,50);

    backButton->hide();

    // =========================
    // 按钮样式
    // =========================

    QString btnStyle =
        "QPushButton{"
        "background-color:#66CCFF;"
        "color:white;"
        "font-size:28px;"
        "border-radius:15px;"
        "border:3px solid white;"
        "}"
        "QPushButton:hover{"
        "background-color:#87CEFA;"
        "}";

    startButton->setStyleSheet(btnStyle);

    guideButton->setStyleSheet(btnStyle);

    exitButton->setStyleSheet(btnStyle);

    // =========================
    // 返回按钮
    // =========================

    backButton->setStyleSheet(
        "QPushButton{"
        "background-color:#444444;"
        "color:white;"
        "font-size:20px;"
        "border-radius:10px;"
        "}"
        );

    // =========================
    // 开始游戏
    // =========================

    connect(startButton,&QPushButton::clicked,[=]()
            {
                gameState = PLAYING;

                startButton->hide();

                guideButton->hide();

                exitButton->hide();

                initGame();

                update();
            });

    // =========================
    // 游戏指南
    // =========================

    connect(guideButton,&QPushButton::clicked,[=]()
            {
                gameState = GUIDE;

                startButton->hide();

                guideButton->hide();

                exitButton->hide();

                backButton->show();

                update();
            });

    // =========================
    // 返回菜单
    // =========================

    connect(backButton,&QPushButton::clicked,[=]()
            {
                gameState = MENU;

                startButton->show();

                guideButton->show();

                exitButton->show();

                backButton->hide();

                update();
            });

    // =========================
    // 退出游戏
    // =========================

    connect(exitButton,&QPushButton::clicked,
            this,&MainWindow::close);

    // =========================
    // 定时器
    // =========================

    timer = new QTimer(this);

    connect(timer,&QTimer::timeout,
            this,&MainWindow::updateGame);

    timer->start(16);

    setFocusPolicy(Qt::StrongFocus);

    // =========================
    // 初始化按键
    // =========================

    aPressed = false;
    dPressed = false;

    leftPressed = false;
    rightPressed = false;
}

MainWindow::~MainWindow()
{
}

void MainWindow::initGame()
{
    // 玩家1
    p1x = 150;
    p1y = 440;
    p1vy = 0;

    // 玩家2
    p2x = 500;
    p2y = 440;
    p2vy = 0;

    // 地图偏移
    mapOffset = 0;

    // 数据
    lives = 3;

    score = 0;

    // 熊大
    xdX = 1000;
    xdY = 440;

    // 熊二
    xeX = 1700;
    xeY = 440;

    // 光头强
    gtqX = 2400;
    gtqY = 440;

    // 房子（真正送回家位置）
    houseX = 3000;
    houseY = 320;

    // 携带状态
    carryingType = 0;

    carrierPlayer = 0;

    // 是否已回家
    xdSaved = false;

    xeSaved = false;

    gtqSaved = false;
}

void MainWindow::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    // =========================
    // 主菜单
    // =========================

    if(gameState == MENU)
    {
        painter.drawPixmap(
            0,
            0,
            800,
            600,
            startCover
            );

        painter.fillRect(
            rect(),
            QColor(0,0,0,80)
            );

        painter.setPen(Qt::white);

        painter.setFont(
            QFont("微软雅黑",40,QFont::Bold)
            );

        painter.drawText(
            170,
            140,
            ""
            );
    }

    // =========================
    // 游戏指南
    // =========================

    else if(gameState == GUIDE)
    {
        painter.drawPixmap(
            0,
            0,
            800,
            600,
            startCover
            );

        painter.fillRect(
            rect(),
            QColor(0,0,0,180)
            );

        painter.setPen(Qt::white);

        painter.setFont(
            QFont("微软雅黑",32,QFont::Bold)
            );

        painter.drawText(
            280,
            100,
            "游戏指南"
            );

        painter.setFont(
            QFont("微软雅黑",20)
            );

        painter.drawText(
            100,
            220,
            "玩家1：W跳跃  A/D移动"
            );

        painter.drawText(
            100,
            300,
            "玩家2：↑跳跃  ←→移动"
            );

        painter.drawText(
            100,
            380,
            "一次只能背一个角色"
            );

        painter.drawText(
            100,
            460,
            "送回光头强的房子获得积分"
            );
    }

    // =========================
    // 游戏场景
    // =========================

    else if(gameState == PLAYING)
    {
        // 无限背景
        for(int i = -1; i < 6; i++)
        {
            painter.drawPixmap(
                i * 800 - (mapOffset % 800),
                0,
                800,
                600,
                background
                );
        }

        // 地面
        painter.fillRect(
            0,
            520,
            800,
            80,
            QColor(120,80,30)
            );

        // UI
        painter.setPen(Qt::white);

        painter.setFont(
            QFont("微软雅黑",22,QFont::Bold)
            );

        painter.drawText(
            20,
            40,
            QString("生命值: %1").arg(lives)
            );

        painter.drawText(
            20,
            80,
            QString("分数: %1").arg(score)
            );

        // =========================
        // 光头强房子
        // =========================

        painter.drawPixmap(
            houseX - mapOffset,
            houseY,
            220,
            220,
            house
            );

        // =========================
        // 熊大
        // =========================

        if(!xdSaved && carryingType != 1)
        {
            painter.drawPixmap(
                xdX - mapOffset,
                xdY,
                60,
                80,
                xiongda
                );
        }

        // =========================
        // 熊二
        // =========================

        if(!xeSaved && carryingType != 2)
        {
            painter.drawPixmap(
                xeX - mapOffset,
                xeY,
                60,
                80,
                xionger
                );
        }

        // =========================
        // 光头强
        // =========================

        if(!gtqSaved && carryingType != 3)
        {
            painter.drawPixmap(
                gtqX - mapOffset,
                gtqY,
                60,
                80,
                guangtouqiang
                );
        }

        // =========================
        // 玩家1
        // =========================

        painter.drawPixmap(
            p1x,
            p1y,
            60,
            80,
            piggy
            );

        // =========================
        // 玩家2
        // =========================

        painter.drawPixmap(
            p2x,
            p2y,
            60,
            80,
            lulu
            );

        // =========================
        // 背负角色
        // =========================

        int carryX;
        int carryY;

        if(carrierPlayer == 1)
        {
            carryX = p1x;
            carryY = p1y;
        }
        else
        {
            carryX = p2x;
            carryY = p2y;
        }

        // 熊大
        if(carryingType == 1)
        {
            painter.drawPixmap(
                carryX,
                carryY - 60,
                50,
                60,
                xiongda
                );
        }

        // 熊二
        if(carryingType == 2)
        {
            painter.drawPixmap(
                carryX,
                carryY - 60,
                50,
                60,
                xionger
                );
        }

        // 光头强
        if(carryingType == 3)
        {
            painter.drawPixmap(
                carryX,
                carryY - 60,
                50,
                60,
                guangtouqiang
                );
        }
    }
}

void MainWindow::updateGame()
{
    if(gameState != PLAYING)
    {
        update();
        return;
    }

    // =========================
    // 玩家1移动
    // =========================

    if(aPressed)
    {
        p1x -= 5;
    }

    if(dPressed)
    {
        p1x += 5;
    }

    // =========================
    // 玩家2移动
    // =========================

    if(leftPressed)
    {
        p2x -= 5;
    }

    if(rightPressed)
    {
        p2x += 5;
    }

    // =========================
    // 镜头
    // =========================

    int centerX = (p1x + p2x) / 2;

    if(centerX > 400)
    {
        mapOffset += 5;

        p1x -= 5;
        p2x -= 5;
    }

    if(centerX < 250 && mapOffset > 0)
    {
        mapOffset -= 5;

        p1x += 5;
        p2x += 5;
    }

    // =========================
    // 重力
    // =========================

    p1vy += 1;
    p2vy += 1;

    p1y += p1vy;
    p2y += p2vy;

    // 地面碰撞

    if(p1y >= 440)
    {
        p1y = 440;
        p1vy = 0;
    }

    if(p2y >= 440)
    {
        p2y = 440;
        p2vy = 0;
    }

    // =========================
    // 世界坐标
    // =========================

    int p1WorldX = p1x + mapOffset;

    int p2WorldX = p2x + mapOffset;

    // =========================
    // 拾取角色
    // =========================

    if(carryingType == 0)
    {
        // 玩家1

        if(!xdSaved && abs(p1WorldX - xdX) < 60)
        {
            carryingType = 1;
            carrierPlayer = 1;
        }

        else if(!xeSaved && abs(p1WorldX - xeX) < 60)
        {
            carryingType = 2;
            carrierPlayer = 1;
        }

        else if(!gtqSaved && abs(p1WorldX - gtqX) < 60)
        {
            carryingType = 3;
            carrierPlayer = 1;
        }

        // 玩家2

        else if(!xdSaved && abs(p2WorldX - xdX) < 60)
        {
            carryingType = 1;
            carrierPlayer = 2;
        }

        else if(!xeSaved && abs(p2WorldX - xeX) < 60)
        {
            carryingType = 2;
            carrierPlayer = 2;
        }

        else if(!gtqSaved && abs(p2WorldX - gtqX) < 60)
        {
            carryingType = 3;
            carrierPlayer = 2;
        }
    }

    // =========================
    // 到达房子
    // =========================

    bool reachHouse = false;

    // 玩家1到家
    if(abs(p1WorldX - houseX) < 180)
    {
        reachHouse = true;
    }

    // 玩家2到家
    if(abs(p2WorldX - houseX) < 180)
    {
        reachHouse = true;
    }

    // 真正送回家
    if(reachHouse && carryingType != 0)
    {
        // 熊大
        if(carryingType == 1)
        {
            xdSaved = true;
        }

        // 熊二
        if(carryingType == 2)
        {
            xeSaved = true;
        }

        // 光头强
        if(carryingType == 3)
        {
            gtqSaved = true;
        }

        // 加分
        score += 100;

        // 清空携带
        carryingType = 0;

        carrierPlayer = 0;
    }

    update();
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    // 玩家1

    if(event->key() == Qt::Key_A)
        aPressed = true;

    if(event->key() == Qt::Key_D)
        dPressed = true;

    if(event->key() == Qt::Key_W)
    {
        if(p1vy == 0)
            p1vy = -15;
    }

    // 玩家2

    if(event->key() == Qt::Key_Left)
        leftPressed = true;

    if(event->key() == Qt::Key_Right)
        rightPressed = true;

    if(event->key() == Qt::Key_Up)
    {
        if(p2vy == 0)
            p2vy = -15;
    }
}

void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_A)
        aPressed = false;

    if(event->key() == Qt::Key_D)
        dPressed = false;

    if(event->key() == Qt::Key_Left)
        leftPressed = false;

    if(event->key() == Qt::Key_Right)
        rightPressed = false;
}