#include "singleplayerpausegameui.h"
#include "singleplayerlevel.h"
#include "dualplayerlevel.h"
#include "singlematchercontroller.h"
#include "dualmatchercontroller.h"
#include "mainwindow.h"
#include "serialization.h"
#include <QFile>

SinglePlayerPauseGameUI::SinglePlayerPauseGameUI(PlayerController *control, QWidget *parent)
    : UserInterface(control, parent)
{
    qDebug() << "Process: single player pause ui construction";
    ui.setupUi(this);
    // 绑定对应按键事件
    connect(ui.startMenuButton, &QPushButton::clicked, MainWindow::getInstance(), &MainWindow::openStartMenuLevel);
    connect(ui.saveButton, &QPushButton::clicked, this, &SinglePlayerPauseGameUI::saveData);
    connect(ui.continueButton, &QPushButton::clicked, static_cast<TileLevel*>(controller->getLevel()), &TileLevel::toggleGamePause);
}

void SinglePlayerPauseGameUI::advance()
{

}

void SinglePlayerPauseGameUI::saveData()
{
    qDebug() << "Process: single player pause game ui save data";
    // 请确保输入的路径为按 '/' 而不是 '\\' 分割的路径
    QString filename {ui.plainTextEdit->toPlainText()};
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly))
        qDebug() << "Process: single player pause game ui save file not exist:" << filename;
    else {
        QDataStream out(&file);
        out.setVersion(QDataStream::Qt_5_15);
        SingleMatcherController *player1 = dynamic_cast<SingleMatcherController*>(controller);
        SinglePlayerLevel *level1;
        // try single level
        if (player1 && (level1 = dynamic_cast<SinglePlayerLevel*>(player1->getLevel()))) {
            Serialization::getInstance()->SerializeSinglePlayer(player1, level1, out);
        } else
            qDebug() << "Process: player pause game ui detect scene not single level";
        // try dual level
        DualMatcherController *player2 = dynamic_cast<DualMatcherController*>(controller);
        DualPlayerLevel *level2;
        if (player2 && (level2 = dynamic_cast<DualPlayerLevel*>(player2->getLevel()))) {
            Serialization::getInstance()->SerializeDualPlayer(player2, level2, out);
        } else
            qDebug() << "Process: player pause game ui detect scene not dual level";
        file.close();
    }
    qDebug() << "Process: single player pause game ui save data finished";
}
