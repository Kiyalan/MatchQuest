#include "mainwindow.h"
#include "startmenulevel.h"
#include "singleplayerlevel.h"
#include "dualplayerlevel.h"
#include "playercontroller.h"
#include "singlematchercontroller.h"
#include "dualmatchercontroller.h"
#include "startmenuplayercontroller.h"
#include "serialization.h"
#include <QApplication>
#include <QLayout>
#include <QFile>

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
    // 设置主窗口的布局
    layout = new QVBoxLayout(this);
    layout->setAlignment(Qt::AlignCenter);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    // 显示窗口。w.show() 只是把显示请求添加到事件队列，由于需要调整窗口大小，而 w.frameGeometry() 在窗口未显示
    // 时并不会返回正确的结果，所以必须显示调用 QApplication::processEvents() 然后再创建 player controller 对象
    show();
    QApplication::processEvents();
}

MainWindow::~MainWindow()
{
    // if (player) player->deleteLater();
    // if (layout) layout->deleteLater();
    // if (level) level->deleteLater();
    delete player;
    delete layout;
    delete level;
}

MainWindow* MainWindow::getInstance()
{
    static MainWindow *ins = new MainWindow;
    return ins;
}

void MainWindow::openStartMenuLevel()
{
    qDebug() << "Process: main window open start menu level";
    delete level;
    level = new StartMenuLevel(this);
    delete player;
    player = new StartMenuPlayerController(this);
    layout->addWidget(player->view);
    bindPlayerToLevel();
    qDebug() << "Process: main window open start menu level completed";
}

void MainWindow::openSinglePlayerLevel()
{
    qDebug() << "Process: main window open single player level";
    delete level;
    level = new SinglePlayerLevel(this, inputWindowHeight, inputWindowWidth);
    // 创建角色控制器，并将其绑定到该窗口
    delete player;
    player = new SingleMatcherController(this);
    layout->addWidget(player->view);
    bindPlayerToLevel();
    qDebug() << "Process: main window open single player level completed";
}

void MainWindow::openDualPlayerLevel()
{
    qDebug() << "Process: main window open dual player level";
    delete level;
    level = new DualPlayerLevel(this, inputWindowHeight, inputWindowWidth);
    delete player;
    player = new DualMatcherController(this);
    layout->addWidget(player->view);
    bindPlayerToLevel();
    qDebug() << "Process: main window open dual player level completed";
}

void MainWindow::bindPlayerToLevel()
{
    player->view->setScene(level);
    player->level = level;
    player->puppets = level->puppets;
    player->cameraPivot = level->cameraPivot;
    level->controller = player;
    player->enterLevelInitialization();
    level->levelInitialization();
}

void MainWindow::loadLevelFromFile()
{
    qDebug() << "Process: single player pause game ui save data";
    // 请确保输入的路径为按 '/' 而不是 '\\' 分割的路径
    QString filename {inputLevelPath};
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly))
        qDebug() << "Process: main window load level from file, file not exist:" << filename;
    else {
        QDataStream in(&file);
        int id;
        in >> id;
        if (id == 1) {
            openSinglePlayerLevel();
            SingleMatcherController *player1 = dynamic_cast<SingleMatcherController*>(player);
            SinglePlayerLevel *level1 = dynamic_cast<SinglePlayerLevel*>(level);
            if (player1 && level1) {
                Serialization::getInstance()->DeserializeSinglePlayer(player1, level1, in);
            } else
                qDebug() << "Process: mainwindow load level from file detect scene not single level";
        } else if (id == 2){
            openDualPlayerLevel();
            DualMatcherController *player2 = dynamic_cast<DualMatcherController*>(player);
            DualPlayerLevel *level2 = dynamic_cast<DualPlayerLevel*>(level);
            if (player2 && level2) {
                Serialization::getInstance()->DeserializeDualPlayer(player2, level2, in);
            } else
                qDebug() << "Process: player pause game ui detect scene not dual level";
        } else
            qDebug() << "Process: main window load level from file, file type not recognized";
        file.close();
    }
    qDebug() << "Process: single player pause game ui save data finished";
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    player->onKeyPressed(event);
}

void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
    player->onKeyReleased(event);
}
