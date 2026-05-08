#include "mainwindow.h"
#include <QPainter>
#include <QKeyEvent>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), gameState(MENU), fartTimer1(0), fartTimer2(0)
{
    resize(800, 600);
    setFixedSize(800, 600);
    setWindowTitle("猪猪侠双人冒险");

    loadImages();
    initUI();

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::updateGame);
    timer->start(16);

    setFocusPolicy(Qt::StrongFocus);
}

MainWindow::~MainWindow() {}

void MainWindow::loadImages() {
    // 使用与基础代码相同的相对路径
    background = QPixmap("res/background.png");
    piggy = QPixmap("res/piggy.png");
    lulu = QPixmap("res/lulu.png");
    xiongda = QPixmap("res/xiongda.png");
    xionger = QPixmap("res/xionger.png");
    guangtouqiang = QPixmap("res/guangtouqiang.png");
    housePic = QPixmap("res/house.png");
    zombiePic = QPixmap("res/zombie.png");
    obstaclePic = QPixmap("res/obstacle.png");
    startCover = QPixmap("res/start_cover.png");   // 你的开始封面图片
    guidePage = QPixmap("res/guide_page.png");
    levelMapPic = QPixmap("res/level_map.png");
    fartEffectPic = QPixmap("res/fart_effect.png");
    heartPic = QPixmap("res/heart.png");
}

void MainWindow::initUI() {
    startButton = new QPushButton("开始游戏", this);
    guideButton = new QPushButton("游戏指南", this);
    exitButton = new QPushButton("退出游戏", this);
    backButton = new QPushButton("返回", this);
    level1Btn = new QPushButton("第1关", this);
    level2Btn = new QPushButton("第2关", this);
    level3Btn = new QPushButton("第3关", this);

    startButton->setGeometry(250, 220, 300, 70);
    guideButton->setGeometry(250, 320, 300, 70);
    exitButton->setGeometry(250, 420, 300, 70);
    backButton->setGeometry(20, 20, 100, 50);
    level1Btn->setGeometry(100, 400, 150, 60);
    level2Btn->setGeometry(325, 400, 150, 60);
    level3Btn->setGeometry(550, 400, 150, 60);

    QString btnStyle = "QPushButton{background-color:#66CCFF;color:white;font-size:28px;border-radius:15px;border:3px solid white;}";
    QString smallStyle = "QPushButton{background-color:#55BB55;color:white;font-size:20px;border-radius:10px;}";
    startButton->setStyleSheet(btnStyle);
    guideButton->setStyleSheet(btnStyle);
    exitButton->setStyleSheet(btnStyle);
    backButton->setStyleSheet(smallStyle);
    level1Btn->setStyleSheet(smallStyle);
    level2Btn->setStyleSheet(smallStyle);
    level3Btn->setStyleSheet(smallStyle);

    backButton->hide();
    level1Btn->hide();
    level2Btn->hide();
    level3Btn->hide();

    connect(startButton, &QPushButton::clicked, [=]() {
        gameState = LEVEL_SELECT;
        startButton->hide(); guideButton->hide(); exitButton->hide();
        backButton->show();
        level1Btn->show(); level2Btn->show(); level3Btn->show();
        update();
    });
    connect(guideButton, &QPushButton::clicked, [=]() {
        gameState = GUIDE;
        startButton->hide(); guideButton->hide(); exitButton->hide();
        backButton->show();
        update();
    });
    connect(exitButton, &QPushButton::clicked, this, &MainWindow::close);
    connect(backButton, &QPushButton::clicked, [=]() {
        if(gameState == LEVEL_SELECT || gameState == GUIDE) {
            gameState = MENU;
            startButton->show(); guideButton->show(); exitButton->show();
            backButton->hide();
            level1Btn->hide(); level2Btn->hide(); level3Btn->hide();
        } else if(gameState == PLAYING || gameState == GAME_OVER || gameState == VICTORY) {
            gameState = MENU;
            startButton->show(); guideButton->show(); exitButton->show();
            backButton->hide();
            level1Btn->hide(); level2Btn->hide(); level3Btn->hide();
            gameWorld.resetToMenu();
        }
        update();
    });
    connect(level1Btn, &QPushButton::clicked, [=]() { onLevelSelected(1); });
    connect(level2Btn, &QPushButton::clicked, [=]() { onLevelSelected(2); });
    connect(level3Btn, &QPushButton::clicked, [=]() { onLevelSelected(3); });
}

void MainWindow::onLevelSelected(int level) {
    gameState = PLAYING;
    gameWorld.init(level);
    fartTimer1 = fartTimer2 = 0;
    level1Btn->hide(); level2Btn->hide(); level3Btn->hide();
    backButton->hide();
    startButton->hide(); guideButton->hide(); exitButton->hide();
    update();
}

void MainWindow::updateGame() {
    if(gameState == PLAYING) {
        // 放屁特效检测
        bool p1Jump = gameWorld.getPlayer1().vy < 0 && !gameWorld.getPlayer1().onGround;
        bool p2Jump = gameWorld.getPlayer2().vy < 0 && !gameWorld.getPlayer2().onGround;
        if(p1Jump && fartTimer1 == 0) {
            fartTimer1 = 5;
            fartPos1 = QPointF(gameWorld.getPlayer1().x, gameWorld.getPlayer1().y + 70);
        }
        if(p2Jump && fartTimer2 == 0) {
            fartTimer2 = 5;
            fartPos2 = QPointF(gameWorld.getPlayer2().x, gameWorld.getPlayer2().y + 70);
        }

        gameWorld.update();

        if(gameWorld.isGameOver()) {
            gameState = GAME_OVER;
            backButton->show();
            startButton->hide(); guideButton->hide(); exitButton->hide();
            level1Btn->hide(); level2Btn->hide(); level3Btn->hide();
        } else if(gameWorld.isVictory()) {
            gameState = VICTORY;
            backButton->show();
            startButton->hide(); guideButton->hide(); exitButton->hide();
            level1Btn->hide(); level2Btn->hide(); level3Btn->hide();
        }

        if(fartTimer1 > 0) fartTimer1--;
        if(fartTimer2 > 0) fartTimer2--;
    }
    update();
}

void MainWindow::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);

    switch(gameState) {
    case MENU:
        painter.drawPixmap(0,0,800,600, startCover);
        painter.fillRect(rect(), QColor(0,0,0,80));
        painter.setPen(Qt::white);
        painter.setFont(QFont("微软雅黑",40,QFont::Bold));
        painter.drawText(170,140,"猪猪侠双人冒险");
        break;
    case GUIDE:
        if(!guidePage.isNull()) painter.drawPixmap(0,0,800,600, guidePage);
        painter.fillRect(rect(), QColor(0,0,0,150));
        painter.setPen(Qt::white);
        painter.setFont(QFont("微软雅黑",32,QFont::Bold));
        painter.drawText(280,100,"游戏指南");
        painter.setFont(QFont("微软雅黑",20));
        painter.drawText(100,220,"玩家1：W跳跃  A/D移动  Q攻击");
        painter.drawText(100,300,"玩家2：↑跳跃  ←→移动  K攻击");
        painter.drawText(100,380,"收集熊大/熊二/光头强并送回房子");
        painter.drawText(100,460,"注意躲避僵尸（可攻击）和障碍物");
        break;
    case LEVEL_SELECT:
        painter.drawPixmap(0,0,800,600, levelMapPic);
        painter.setPen(Qt::white);
        painter.setFont(QFont("微软雅黑",30,QFont::Bold));
        painter.drawText(280,80,"选择关卡");
        break;
    case PLAYING:
        drawGame(painter);
        break;
    case GAME_OVER:
        painter.fillRect(rect(), QColor(0,0,0,200));
        painter.setPen(Qt::red);
        painter.setFont(QFont("微软雅黑",40,QFont::Bold));
        painter.drawText(250,280,"游戏结束");
        painter.setFont(QFont("微软雅黑",20));
        painter.drawText(300,380,"按返回按钮回主菜单");
        break;
    case VICTORY:
        painter.fillRect(rect(), QColor(0,0,0,200));
        painter.setPen(Qt::yellow);
        painter.setFont(QFont("微软雅黑",40,QFont::Bold));
        painter.drawText(220,280,"恭喜通关！");
        break;
    }
}

void MainWindow::drawGame(QPainter &painter) {
    float mapOffset = gameWorld.getMapOffset();
    // 无限背景滚动
    for(int i = -1; i < 6; ++i) {
        painter.drawPixmap(i*800 - (int(mapOffset)%800), 0, 800, 600, background);
    }
    // 地面（草绿色）
    painter.fillRect(0,520,800,80, QColor(100,180,70));

    // 平台
    painter.setBrush(QColor(139,69,19));
    painter.setPen(Qt::NoPen);
    for(auto &plat : gameWorld.getPlatforms()) {
        painter.drawRect(plat.rect().translated(-mapOffset, 0));
    }

    // 障碍物
    for(auto &obs : gameWorld.getObstacles()) {
        painter.drawPixmap(obs.x - mapOffset, obs.y, 60, 45, obstaclePic);
    }

    // 僵尸
    for(auto &z : gameWorld.getZombies()) {
        if(z.alive)
            painter.drawPixmap(z.x - mapOffset, z.y, 50, 70, zombiePic);
    }

    // 可拾取物
    for(auto &c : gameWorld.getCollectibles()) {
        if(!c.exists) continue;
        QPixmap pic = (c.type == XIONGDA ? xiongda : (c.type == XIONGER ? xionger : guangtouqiang));
        painter.drawPixmap(c.x - mapOffset, c.y, 50, 60, pic);
    }

    // 房子
    painter.drawPixmap(gameWorld.getHouseX() - mapOffset, gameWorld.getHouseY(), 150, 150, housePic);

    // 玩家
    drawPlayerWithCarry(painter, gameWorld.getPlayer1(), piggy);
    drawPlayerWithCarry(painter, gameWorld.getPlayer2(), lulu);

    // 放屁特效
    if(fartTimer1 > 0)
        painter.drawPixmap(fartPos1.x() - mapOffset - 20, fartPos1.y(), 50, 50, fartEffectPic);
    if(fartTimer2 > 0)
        painter.drawPixmap(fartPos2.x() - mapOffset - 20, fartPos2.y(), 50, 50, fartEffectPic);

    // 生命值心形
    int lives = gameWorld.getLives();
    for(int i=0; i<lives; ++i)
        painter.drawPixmap(20 + i*40, 20, 35, 35, heartPic);

    // 关卡显示
    painter.setPen(Qt::white);
    painter.setFont(QFont("微软雅黑",20));
    painter.drawText(680,50, QString("第%1关").arg(gameWorld.getCurrentLevel()));
}

void MainWindow::drawPlayerWithCarry(QPainter &painter, const Player &p, const QPixmap &basePic) {
    float mapOffset = gameWorld.getMapOffset();
    float drawX = p.x - mapOffset;
    painter.drawPixmap(drawX, p.y, 60, 80, basePic);
    if(p.carryCount > 0) {
        QPixmap carryPic;
        if(p.carryXiongDa) carryPic = xiongda;
        else if(p.carryXiongEr) carryPic = xionger;
        else carryPic = guangtouqiang;
        painter.drawPixmap(drawX+5, p.y-50, 50, 60, carryPic);
    }
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
    if(gameState != PLAYING) return;
    bool a=false,d=false,left=false,right=false,w=false,up=false,q=false,k=false;
    switch(event->key()) {
    case Qt::Key_A: a=true; break;
    case Qt::Key_D: d=true; break;
    case Qt::Key_W: w=true; break;
    case Qt::Key_Left: left=true; break;
    case Qt::Key_Right: right=true; break;
    case Qt::Key_Up: up=true; break;
    case Qt::Key_Q: q=true; break;
    case Qt::Key_K: k=true; break;
    default: return;
    }
    gameWorld.setInput(a,d,left,right,w,up,q,k);
}

void MainWindow::keyReleaseEvent(QKeyEvent *event) {
    if(gameState != PLAYING) return;
    bool a=false,d=false,left=false,right=false,w=false,up=false,q=false,k=false;
    switch(event->key()) {
    case Qt::Key_A: a=false; break;
    case Qt::Key_D: d=false; break;
    case Qt::Key_W: w=false; break;
    case Qt::Key_Left: left=false; break;
    case Qt::Key_Right: right=false; break;
    case Qt::Key_Up: up=false; break;
    case Qt::Key_Q: q=false; break;
    case Qt::Key_K: k=false; break;
    default: return;
    }
    gameWorld.setInput(a,d,left,right,w,up,q,k);
}