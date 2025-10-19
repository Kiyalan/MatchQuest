#include "dualplayeringameui.h"
#include "../../resource/ui/ui_dualplayeringameui.h"
#include "dualmatchercontroller.h"
#include "dualplayerlevel.h"
#include <QString>

DualPlayerInGameUI::DualPlayerInGameUI(PlayerController* contr, QWidget *parent)
    : UserInterface(contr, parent)
{
    ui.setupUi(this);
}

void DualPlayerInGameUI::advance()
{
    auto currentLevel = static_cast<DualPlayerLevel*>(controller->getLevel());
    if (!currentLevel)
        qFatal() << "Fatal: DualPlayerInGameUI::advance current level not dual player level or empty";
    int downcount = currentLevel->getDowncount();
    int score1 = currentLevel->getScore(0);
    int score2 = currentLevel->getScore(1);
    ui.timeValueLabel->setText(QString::number(downcount));
    ui.score1ValueLabel->setText(QString::number(score1));
    ui.score2ValueLabel->setText(QString::number(score2));
    update();
}


