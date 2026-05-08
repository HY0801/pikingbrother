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
    bg1 = QPixmap("res/background1.png");
    bg2 = QPixmap("res/background2.png");
    bg3 = QPixmap("res/background3.png");

    piggy = QPixmap("res/piggy.png");
    lulu = QPixmap("res/lulu.png");

    xiongda = QPixmap("res/xiongda.png");
    xionger = QPixmap("res/xionger.png");
    guangtouqiang = QPixmap("res/guangtouqiang.png");

    tutu = QPixmap("res/tutu.png");
    xiaomei = QPixmap("res/xiaomei.png");
    xiaoguai = QPixmap("res/xiaoguai.png");
    shuazi = QPixmap("res/shuazi.png");

    xiyangyang = QPixmap("res/xiyangyang.png");
    meiyangyang = QPixmap("res/meiyangyang.png");
    nuanyangyang = QPixmap("res/nuanyangyang.png");
    feiyangyang = QPixmap("res/feiyangyang.png");
    lanyangyang = QPixmap("res/lanyangyang.png");

    housePic = QPixmap("res/house.png");
    house2Pic = QPixmap("res/house2.png");
    house3Pic = QPixmap("res/house3.png");

    zombiePic = QPixmap("res/zombie.png");
    tutumaPic = QPixmap("res/tutuma.png");
    huitailangPic = QPixmap("res/huitailang.png");

    obstaclePic = QPixmap("res/obstacle.png");
    startCover = QPixmap("res/start_cover.png");
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
    nextLevelButton = new QPushButton("下一关", this);
    exitAfterLevelButton = new QPushButton("退出到菜单", this);

    startButton->setGeometry(250, 220, 300, 70);
    guideButton->setGeometry(250, 320, 300, 70);
    exitButton->setGeometry(250, 420, 300, 70);
    backButton->setGeometry(20, 20, 100, 50);
    level1Btn->setGeometry(100, 420, 150, 60);
    level2Btn->setGeometry(300, 300, 150, 60);
    level3Btn->setGeometry(550, 180, 150, 60);
    nextLevelButton->setGeometry(300, 350, 200, 60);
    exitAfterLevelButton->setGeometry(300, 450, 200, 60);

    QString btnStyle = "QPushButton{background-color:#A8E6A3;color:white;font-size:28px;border-radius:15px;border:3px solid white;}";
    QString smallStyle = "QPushButton{background-color:#A8E6A3;color:white;font-size:20px;border-radius:10px;border:2px solid white;}";
    QString backWhiteStyle = "QPushButton{background-color:#444444;color:white;font-size:20px;border-radius:10px;}";

    startButton->setStyleSheet(btnStyle);
    guideButton->setStyleSheet(btnStyle);
    exitButton->setStyleSheet(btnStyle);
    backButton->setStyleSheet(backWhiteStyle);
    level1Btn->setStyleSheet(smallStyle);
    level2Btn->setStyleSheet(smallStyle);
    level3Btn->setStyleSheet(smallStyle);
    nextLevelButton->setStyleSheet(smallStyle);
    exitAfterLevelButton->setStyleSheet(smallStyle);

    backButton->hide();
    level1Btn->hide();
    level2Btn->hide();
    level3Btn->hide();
    nextLevelButton->hide();
    exitAfterLevelButton->hide();

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
        } else if(gameState == PLAYING || gameState == GAME_OVER || gameState == VICTORY || gameState == LEVEL_COMPLETE) {
            gameState = MENU;
            startButton->show(); guideButton->show(); exitButton->show();
            backButton->hide();
            level1Btn->hide(); level2Btn->hide(); level3Btn->hide();
            nextLevelButton->hide(); exitAfterLevelButton->hide();
            gameWorld.resetToMenu();
        }
        update();
    });
    connect(level1Btn, &QPushButton::clicked, [=]() { onLevelSelected(1); });
    connect(level2Btn, &QPushButton::clicked, [=]() { onLevelSelected(2); });
    connect(level3Btn, &QPushButton::clicked, [=]() { onLevelSelected(3); });
    connect(nextLevelButton, &QPushButton::clicked, this, &MainWindow::onNextLevel);
    connect(exitAfterLevelButton, &QPushButton::clicked, this, &MainWindow::onExitToMenu);
}

void MainWindow::onLevelSelected(int level) {
    gameState = PLAYING;
    gameWorld.init(level);
    fartTimer1 = fartTimer2 = 0;
    level1Btn->hide(); level2Btn->hide(); level3Btn->hide();
    backButton->hide();
    startButton->hide(); guideButton->hide(); exitButton->hide();
    gameWorld.setInput(false, false, false, false, false, false, false, false);
    update();
}

void MainWindow::onNextLevel() {
    gameWorld.nextLevel();
    gameState = PLAYING;
    nextLevelButton->hide();
    exitAfterLevelButton->hide();
    backButton->hide();
    gameWorld.setInput(false, false, false, false, false, false, false, false);
    update();
}

void MainWindow::onExitToMenu() {
    gameState = MENU;
    startButton->show(); guideButton->show(); exitButton->show();
    backButton->hide();
    nextLevelButton->hide(); exitAfterLevelButton->hide();
    gameWorld.resetToMenu();
    update();
}

void MainWindow::updateGame() {
    if(gameState == PLAYING) {
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
            nextLevelButton->hide(); exitAfterLevelButton->hide();
        } else if(gameWorld.isVictory()) {
            gameState = VICTORY;
            backButton->show();
            startButton->hide(); guideButton->hide(); exitButton->hide();
            level1Btn->hide(); level2Btn->hide(); level3Btn->hide();
        } else if(gameWorld.isLevelComplete()) {
            gameState = LEVEL_COMPLETE;
            nextLevelButton->show();
            exitAfterLevelButton->show();
            backButton->hide();
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
        painter.drawText(100,380,"你们的任务就是拾取童年回忆");
        painter.drawText(100,460,"小心有人会攻击你，你也可以反击");
        break;
    case LEVEL_SELECT:
        painter.drawPixmap(0,0,800,600, levelMapPic);
        painter.setPen(QColor(255, 255, 0)); // 荧光黄
        painter.setFont(QFont("微软雅黑", 36, QFont::Bold));
        // 顶部居中：y = 60
        painter.drawText(QRect(0, 60, 800, 100), Qt::AlignCenter, "选择关卡");
        break;
    case PLAYING:
        drawGame(painter);
        break;
    case LEVEL_COMPLETE:
        drawGame(painter);
        painter.fillRect(rect(), QColor(0,0,0,180));
        painter.setPen(Qt::yellow);
        painter.setFont(QFont("微软雅黑",32,QFont::Bold));
        painter.drawText(250,200,"关卡完成！");
        painter.setFont(QFont("微软雅黑",20));
        painter.drawText(280,300,"点击下方按钮继续");
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
    int level = gameWorld.getCurrentLevel();

    QPixmap currentBg;
    if (level == 1) currentBg = bg1;
    else if (level == 2) currentBg = bg2;
    else currentBg = bg3;
    if (currentBg.isNull()) {
        painter.fillRect(0,0,800,600, QColor(50,150,50));
    } else {
        int bgOffset = int(mapOffset) % 800;
        for(int i = -1; i < 6; ++i) {
            painter.drawPixmap(i*800 - bgOffset, 0, 800, 600, currentBg);
        }
    }

    QColor groundColor, platformColor;
    if (level == 1) {
        groundColor = QColor(0, 100, 0);
        platformColor = QColor(0, 100, 0);
    } else if (level == 2) {
        groundColor = QColor(200, 200, 200);
        platformColor = QColor(200, 200, 200);
    } else {
        groundColor = QColor(245, 222, 179);
        platformColor = QColor(245, 222, 179);
    }
    painter.fillRect(0,520,800,80, groundColor);

    painter.setBrush(platformColor);
    painter.setPen(Qt::NoPen);
    for(auto &plat : gameWorld.getPlatforms()) {
        QRectF screenRect = plat.rect();
        screenRect.translate(-mapOffset, 0);
        painter.drawRect(screenRect);
    }

    for(auto &obs : gameWorld.getObstacles()) {
        painter.drawPixmap(obs.x - mapOffset, obs.y, 60, 45, obstaclePic);
    }

    for(auto &z : gameWorld.getZombies()) {
        if(z.alive) {
            if (level == 1) painter.drawPixmap(z.x - mapOffset, z.y, 50, 70, zombiePic);
            else if (level == 2) painter.drawPixmap(z.x - mapOffset, z.y, 50, 70, tutumaPic);
            else painter.drawPixmap(z.x - mapOffset, z.y, 50, 70, huitailangPic);
        }
    }

    for(auto &c : gameWorld.getCollectibles()) {
        if(!c.exists) continue;
        QPixmap pic;
        switch(c.type) {
        case XIONGDA: pic = xiongda; break;
        case XIONGER: pic = xionger; break;
        case GUANGTOUQIANG: pic = guangtouqiang; break;
        case TUTU: pic = tutu; break;
        case XIAOMEI: pic = xiaomei; break;
        case XIAOGUAI: pic = xiaoguai; break;
        case SHUAZI: pic = shuazi; break;
        case XIYANGYANG: pic = xiyangyang; break;
        case MEIYANGYANG: pic = meiyangyang; break;
        case NUANYANGYANG: pic = nuanyangyang; break;
        case FEIYANGYANG: pic = feiyangyang; break;
        case LANYANGYANG: pic = lanyangyang; break;
        default: continue;
        }
        painter.drawPixmap(c.x - mapOffset, c.y, 50, 60, pic);
    }

    if (level == 1) painter.drawPixmap(gameWorld.getHouseX() - mapOffset, gameWorld.getHouseY(), 150, 150, housePic);
    else if (level == 2) painter.drawPixmap(gameWorld.getHouseX() - mapOffset, gameWorld.getHouseY(), 150, 150, house2Pic);
    else painter.drawPixmap(gameWorld.getHouseX() - mapOffset, gameWorld.getHouseY(), 150, 150, house3Pic);

    float p1x = gameWorld.getPlayer1().x - mapOffset;
    float p1y = gameWorld.getPlayer1().y;
    float p2x = gameWorld.getPlayer2().x - mapOffset;
    float p2y = gameWorld.getPlayer2().y;
    painter.drawPixmap(p1x, p1y, 60, 80, piggy);
    painter.drawPixmap(p2x, p2y, 60, 80, lulu);

    if(gameWorld.getPlayer1().carryCount > 0) {
        int type = gameWorld.getPlayer1().carryType;
        QPixmap carryPic;
        switch(type) {
        case XIONGDA: carryPic = xiongda; break;
        case XIONGER: carryPic = xionger; break;
        case GUANGTOUQIANG: carryPic = guangtouqiang; break;
        case TUTU: carryPic = tutu; break;
        case XIAOMEI: carryPic = xiaomei; break;
        case XIAOGUAI: carryPic = xiaoguai; break;
        case SHUAZI: carryPic = shuazi; break;
        case XIYANGYANG: carryPic = xiyangyang; break;
        case MEIYANGYANG: carryPic = meiyangyang; break;
        case NUANYANGYANG: carryPic = nuanyangyang; break;
        case FEIYANGYANG: carryPic = feiyangyang; break;
        case LANYANGYANG: carryPic = lanyangyang; break;
        default: carryPic = QPixmap();
        }
        if (!carryPic.isNull())
            painter.drawPixmap(p1x+5, p1y-50, 50, 60, carryPic);
    }
    if(gameWorld.getPlayer2().carryCount > 0) {
        int type = gameWorld.getPlayer2().carryType;
        QPixmap carryPic;
        switch(type) {
        case XIONGDA: carryPic = xiongda; break;
        case XIONGER: carryPic = xionger; break;
        case GUANGTOUQIANG: carryPic = guangtouqiang; break;
        case TUTU: carryPic = tutu; break;
        case XIAOMEI: carryPic = xiaomei; break;
        case XIAOGUAI: carryPic = xiaoguai; break;
        case SHUAZI: carryPic = shuazi; break;
        case XIYANGYANG: carryPic = xiyangyang; break;
        case MEIYANGYANG: carryPic = meiyangyang; break;
        case NUANYANGYANG: carryPic = nuanyangyang; break;
        case FEIYANGYANG: carryPic = feiyangyang; break;
        case LANYANGYANG: carryPic = lanyangyang; break;
        default: carryPic = QPixmap();
        }
        if (!carryPic.isNull())
            painter.drawPixmap(p2x+5, p2y-50, 50, 60, carryPic);
    }

    if(fartTimer1 > 0)
        painter.drawPixmap(fartPos1.x() - mapOffset - 20, fartPos1.y(), 50, 50, fartEffectPic);
    if(fartTimer2 > 0)
        painter.drawPixmap(fartPos2.x() - mapOffset - 20, fartPos2.y(), 50, 50, fartEffectPic);

    int lives = gameWorld.getLives();
    for(int i=0; i<lives; ++i)
        painter.drawPixmap(20 + i*40, 20, 35, 35, heartPic);

    QVector<int> saved = gameWorld.getSavedList();
    int savedCount = saved.size();
    painter.setFont(QFont("微软雅黑", 16));
    painter.setPen(Qt::white);
    painter.drawText(20, 100, "已解救：");
    for (int i = 0; i < savedCount; ++i) {
        QPixmap icon;
        switch(saved[i]) {
        case XIONGDA: icon = xiongda; break;
        case XIONGER: icon = xionger; break;
        case GUANGTOUQIANG: icon = guangtouqiang; break;
        case TUTU: icon = tutu; break;
        case XIAOMEI: icon = xiaomei; break;
        case XIAOGUAI: icon = xiaoguai; break;
        case SHUAZI: icon = shuazi; break;
        case XIYANGYANG: icon = xiyangyang; break;
        case MEIYANGYANG: icon = meiyangyang; break;
        case NUANYANGYANG: icon = nuanyangyang; break;
        case FEIYANGYANG: icon = feiyangyang; break;
        case LANYANGYANG: icon = lanyangyang; break;
        default: continue;
        }
        if (!icon.isNull())
            painter.drawPixmap(100 + i*45, 80, 35, 35, icon);
    }

    painter.setPen(Qt::white);
    painter.setFont(QFont("微软雅黑",20));
    painter.drawText(680,50, QString("第%1关").arg(gameWorld.getCurrentLevel()));
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