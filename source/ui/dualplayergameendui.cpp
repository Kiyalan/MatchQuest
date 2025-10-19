#include "dualplayergameendui.h"
#include "dualplayerlevel.h"
#include "playercontroller.h"

DualPlayerGameEndUI::DualPlayerGameEndUI(GameEndCause cause, PlayerController *control, QWidget *parent)
    : UserInterface(control, parent)
{
    qDebug() << "Process: dual player game end ui construction";
    ui.setupUi(this);
    // 初始化结束原因、最终分数、剩余时间
    ui.gameEndCauseLabel->setText(gameEndCauseToString(cause));
    auto dualLevel = static_cast<DualPlayerLevel*>(control->getLevel());
    if (dualLevel == nullptr)
        qFatal() << "Fatal: DualPlayerGameEndUI::DualPlayerGameEndUI control->getLevel() empty or not dual player level";
    ui.score1ValueLabel->setText(QString::number(dualLevel->getScore(0)));
    ui.score2ValueLabel->setText(QString::number(dualLevel->getScore(1)));
    ui.restTimeValueLabel->setText(QString::number(dualLevel->getDowncount()));
    connect(ui.startMenuButton, &QPushButton::clicked, MainWindow::getInstance(), &MainWindow::openStartMenuLevel);
    update();
}

void DualPlayerGameEndUI::advance()
{

}
