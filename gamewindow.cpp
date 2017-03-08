#include "gamewindow.h"
#include "ui_gamewindow.h"

GameWindow::GameWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GameWindow)
{
    ui->setupUi(this);
    scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);
//    controls = new QGraphicsTextItem("Controls: ");
//    wsad = new QGraphicsTextItem("WSAD -- move");
//    shdg = new QGraphicsTextItem("K -- shoot L -- dig");
}

GameWindow::~GameWindow()
{
    delete ui;
}

int GameWindow::getPosFromXCoor() {
    return 10 + (xCoors - 1) * (boxWidth + wallWidth);
}

int GameWindow::getPosFromYCoor() {
    return 10 + (yCoors - 1) * (boxWidth + wallWidth);
}

void GameWindow::initialize() {
    //creates a new field for current player
    //menu AlignRight with keys description
    QBrush blueBrush = QBrush(Qt::blue);
    QBrush brush = QBrush(0xcfbea5);

    QPen myPen = QPen(Qt::black);
    myPen.setWidth(5);

    scene->addRect(10, 10, summaryWidth, summaryHeight, myPen, brush);
    menu = scene->addRect(summaryWidth + 50, 10, 200, 500, myPen, QBrush(Qt::gray));

    controls = scene->addText("Controls: ", QFont("Times", 16, QFont::Bold));
    wsad = scene->addText("WSAD -- move");
    sh = scene->addText("K -- shoot");
    dg = scene->addText("L -- dig");
    inventory = scene->addText("Inventory", QFont("Times", 16, QFont::Bold));
    QGraphicsTextItem *keyText = scene->addText("Keys", QFont("Times", 13, QFont::Bold));
    QGraphicsTextItem *bulletText = scene->addText("Bullets", QFont("Times", 13, QFont::Bold));
    QFont font("Times", 16, QFont::Bold);
    treasureText = scene->addText("TREASURE!", font);
    treasureText->setDefaultTextColor(Qt::red);
    keyNum = scene->addText(QString::number(keys), QFont("Times", 16));
    bulletNum = scene->addText(QString::number(bullets), QFont("Times", 16));

    controls->setPos(100 + summaryWidth, 20);
    inventory->setPos(100 + summaryWidth, 120);
    wsad->setPos(90 + summaryWidth, 50);
    sh->setPos(90 + summaryWidth, 70);
    dg->setPos(90 + summaryWidth, 90);
    keyText->setPos(110 + summaryWidth, 170);
    bulletText->setPos(110 + summaryWidth, 220);
    treasureText->setPos(90 + summaryWidth, 400);
    treasureText->hide();
    keyNum->setPos(197 + summaryWidth, 170);
    bulletNum->setPos(197 + summaryWidth, 220);


    for (int i = 0; i < 3; i++) {
        QLineF line(185 + summaryWidth, 160 + i * 50, 235 + summaryWidth, 160 + i * 50);
        scene->addLine(line, QPen(Qt::black));
    }

    for (int i = 0; i < 2; i++) {
        QLineF line(185 + summaryWidth + i * 50, 160, 185 + summaryWidth + i * 50, 260);
        scene->addLine(line, QPen(Qt::black));
    }

    for (int i = 1; i < width; i++) {
        int x = 10 + i * (boxWidth + wallWidth);
        QLineF line(x - wallWidth, 10, x - wallWidth, 10 + summaryHeight);
        QLineF line2(x, 10, x, 10 + summaryHeight);
        scene->addLine(line, myPen);
        scene->addLine(line2, myPen);
    }

    for (int i = 1; i < height; i++) {
        int y = 10 + i * (boxWidth + wallWidth);
        QLineF line(10, y - wallWidth, 10 + summaryWidth, y - wallWidth);
        QLineF line2(10, y, 10 + summaryWidth, y);
        scene->addLine(line, myPen);
        scene->addLine(line2, myPen);
    }
    myPen.setWidth(3);
    playerIcon = scene->addEllipse(getPosFromXCoor() + 5, getPosFromYCoor() + 5, 40, 40, myPen, blueBrush);

}

void GameWindow::setParams(int width, int height, int xCoors, int yCoors, int startAmmo,
               int startLifes, int keys, int bullets, int mines) {
    //Беру ширину и длину поля
    this->width = width;
    this->height = height;
    summaryWidth = width * boxWidth + (width - 1) * wallWidth;
    summaryHeight = height * boxWidth + (height - 1) * wallWidth;
    this->resize(summaryWidth + 300, std::max(summaryHeight, 500) + 50);

    //Беру кооры игрока, в инициализации нужно
    this->xCoors = xCoors;
    this->yCoors = yCoors;


    this->startAmmo = startAmmo;
    this->startLifes = startLifes;
    this->keys = keys;
    this->bullets = bullets;
    this->mines = mines;

    initialize(); //собсна инициализируем поле. вызывается один раз, когда игрок коннектится
}

void GameWindow::setParamsFloat(float wallProb, float staticTreasureProb, float loveToiletsProb,
                    bool canPutTreasureTogether, bool useRandomTreasure) {
    this->wallProb = wallProb;
    this->staticTreasureProb = staticTreasureProb;
    this->loveToiletsProb = loveToiletsProb;
    this->canPutTreasureTogether = canPutTreasureTogether;
    this->useRandomTreasure = useRandomTreasure;
}

void GameWindow::keyPressEvent(QKeyEvent *key) {
    switch (key->key()) {
        case Qt::Key_Escape:
            close();
            break;
        case Qt::Key_W:
            move(1);
            break;
        case Qt::Key_D:
            move(2);
            break;
        case Qt::Key_A:
            move(0);
            break;
        case Qt::Key_S:
            move(3);
            break;
        case Qt::Key_K:
            shoot();
            break;
        case Qt::Key_L:
            dig();
            break;
    }
}

void GameWindow::update() {
    playerXCoor = getPosFromXCoor();
    playerYCoor = getPosFromYCoor();
    QPen myPen = QPen(Qt::black);
    myPen.setWidth(3);
    QBrush blueBrush = QBrush(Qt::blue);
    scene->removeItem((QGraphicsItem*) playerIcon);
    playerIcon = scene->addEllipse(playerXCoor + 5, playerYCoor + 5, 40, 40, myPen, blueBrush);
}

void GameWindow::updateInfo() {
    scene->removeItem((QGraphicsItem*) keyNum);
    scene->removeItem((QGraphicsItem*) bulletNum);
    keyNum = scene->addText(QString::number(keys), QFont("Times", 16));
    bulletNum = scene->addText(QString::number(bullets), QFont("Times", 16));
    if (check() == 1) {
        showTreasureText();
    } else {
        hideTreasureText();
    }
}

void GameWindow::move(int direction) {
    int i = movePlayer(direction);
    switch (i) {
        case 0:
            switch (direction) {
            case 0:
                xCoors--;
                break;
            case 1:
                yCoors--;
                break;
            case 2:
                xCoors++;
                break;
            case 3:
                yCoors++;
                break;
            }

            update();
            break;
        case 1:
            drawWall(xCoors, yCoors, direction);
            break;
        case 2:
            switch (direction) {
            case 0:
                xCoors--;
                break;
            case 1:
                yCoors--;
                break;
            case 2:
                xCoors++;
                break;
            case 3:
                yCoors++;
                break;
            }

        update();
            showTreasureText();
            break;
        case 3:
            //player life's -1 and checkIfDead()
            break;
    }
}

int GameWindow::movePlayer(int direction) { //direction: 0 - left, 1 - up, 2 - right, 3 - down
    //return 0 if player can get to that direction
    //return 1 if there is a wall
    //return 2 if we can move and there is a treasure
    //return 3 if we can move and there is a mine
    return 0;
}

int GameWindow::check() {
    //ask server if there something at this position
    //return 0 if there is nothing
    //return 1 if there is a treasure
    //return 2 if there is a mine
    return 0;
}

void GameWindow::drawWall(int curXCoor, int curYCoor, int direction) {
    if ((curXCoor == 1 && direction == 0) || (curXCoor == width && direction == 2) ||
            (curYCoor == 1 && direction == 1) || (curYCoor == height && direction == 3)) {
        return;
    }
    QPen pen = QPen(Qt::black);
    pen.setWidth(10);
    int x1;
    int y1;
    int x2;
    int y2;
    switch (direction) {
        case 0:
            x1 = getPosFromXCoor() - 5;
            y1 = getPosFromYCoor() - 5;
            x2 = x1;
            y2 = y1 + 60;
            break;
        case 1:
            x1 = getPosFromXCoor() - 5;
            y1 = getPosFromYCoor() - 5;
            x2 = x1 + 60;
            y2 = y1;
            break;
        case 2:
            x1 = getPosFromXCoor() + 55;
            y1 = getPosFromYCoor() - 5;
            x2 = x1;
            y2 = y1 + 60;
            break;
        case 3:
            x1 = getPosFromXCoor() - 5;
            y1 = getPosFromYCoor() + 55;
            x2 = x1 + 60;
            y2 = y1;
            break;
    }
    QLineF line(x1, y1, x2, y2);
    scene->addLine(line, pen);
}

void GameWindow::shoot() {
    //if killed someone then showKillText()
}

void GameWindow::dig() {
    //+1 to players stash
    //remove treasure sector from this position

    hideTreasureText();
}

void GameWindow::showTreasureText() {
    treasureText->show();
}

void GameWindow::hideTreasureText() {
    treasureText->hide();
}
