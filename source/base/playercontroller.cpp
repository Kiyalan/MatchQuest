#include "playercontroller.h"
#include "mainwindow.h"
#include "character.h"
#include "userinterface.h"
#include "emptygraphicsitem.h"
#include <QKeyEvent>
#include <QString>
#include <QRect>
#include <QGraphicsView>
#include <QScrollBar>

PlayerController::PlayerController(MainWindow* iwindow)
    : QObject(iwindow), window(iwindow)
{
    // 重新设定窗口大小，不需要考虑窗口边框大小
    // int frameWidth = window->frameGeometry().width() - window->width();
    // int frameHeight = window->frameGeometry().height() - window->height();
    // window->resize(frameWidth + clientAreaWidth, frameHeight + clientAreaHeight);
    window->resize(clientAreaWidth, clientAreaHeight);
    // 创建 view 作为 MainWindow 的子控件并设定属性
    view = new QGraphicsView(window);
    // 完全禁用 scroll bar 的功能和显示
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->horizontalScrollBar()->setDisabled(true);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->verticalScrollBar()->setDisabled(true);
    // 启用 FullViewport 全视图更新重绘而不是部分更新重绘，避免角色移动残留白边
    view->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    // 设定 view 为固定大小，view 内的 viewport 和 view 的布局不使用 QLayout 实现所以其本身不支持 margin 进而无需清除
    // 所以 view 大小、view 内的 viewport 大小、窗口大小在初始时都恰好为设定的用户区大小

    // 使 view 的视口居中对齐（在 sceneRect 小于 view geometry 时）
    view->setFixedSize(viewWidth, viewHeight);
    view->setAlignment(Qt::AlignCenter);
    // 开启抗锯齿
    view->setRenderHint(QPainter::Antialiasing);
    view->setRenderHint(QPainter::TextAntialiasing);
    // 显示
    view->show();
    // 绑定打开关卡的 signal/slot
}

PlayerController::~PlayerController()
{
    delete view;
    // for (auto &x : hudList)
    //     delete x;
}

void PlayerController::enterLevelInitialization()
{
    for (auto & x : puppets)
        x->controller = this;
}

void PlayerController::onKeyPressed(QKeyEvent* event)
{

}
void PlayerController::onKeyReleased(QKeyEvent* event)
{

}

void PlayerController::advance()
{
    // qDebug() << "Process: player controller advance";
    updateViewPosition();
    for (auto &x : hudList)
        x->advance();
    customAdvance();
}

void PlayerController::customAdvance()
{

}

void PlayerController::updateViewPosition()
{
    QRectF viewArea {0., 0., viewWidth * 1., viewHeight * 1.};
    viewArea.moveCenter(cameraPivot->pos());
    view->setSceneRect(viewArea);
}

void PlayerController::showUserInterface(UserInterface* ui)
{
    ui->setParent(view);
    ui->setFixedSize(viewWidth, viewHeight);
    ui->show();
    hudList.append(ui);
}
