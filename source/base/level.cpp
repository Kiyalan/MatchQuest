#include "level.h"
#include "playercontroller.h"
#include "configuration.h"
#include "emptygraphicsitem.h"
#include <QGraphicsObject>
#include <QGraphicsRectItem>
#include <QTimer>

Level::Level(MainWindow* window)
    : QGraphicsScene(window)
{
    setBackgroundBrush(Configuration::sceneBackgroundColor);
    // 初始化 root 和 cameraPivot
    root = new EmptyGraphicsItem;
    addItem(root);
    root->setPos(0, 0);
    // 可在子类构造函数中修改 cameraPivot 的初始位置，默认为原点
    cameraPivot = new EmptyGraphicsItem(root);
    cameraPivot->setPos(0, 0);
    // 定时调用 advance()
    auto timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Level::advanceCall);
    timer->start(Configuration::milisecondsPerFrame);
}

Level::~Level()
{
    delete root;
}

void Level::levelInitialization()
{

}

void Level::advanceCall()
{
    customAdvance();
}

void Level::advance()
{
    QList<QGraphicsItem*> actors = items();
    for (int i = 0; i < 2; ++i)
        for (auto &x : actors) {
            if (!x)
                qFatal() << "Fatal: Level::advance actor empty" << x;
            else
                // qDebug() << "    Detail: level advance call actor" << x;
                ;
            x->advance(i);
        }
}

void Level::customAdvance()
{
    controller->advance();
    advance();
}

