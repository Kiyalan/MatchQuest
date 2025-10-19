#include "singleplayeringameui.h"
#include "../../resource/ui/ui_singleplayeringameui.h"
#include "singlematchercontroller.h"
#include "singleplayerlevel.h"
#include <QString>

SinglePlayerInGameUI::SinglePlayerInGameUI(PlayerController* contr, QWidget *parent)
    : UserInterface(contr, parent)
{
    ui.setupUi(this);
}

void SinglePlayerInGameUI::advance()
{
    auto currentLevel = static_cast<SinglePlayerLevel*>(controller->getLevel());
    if (!currentLevel)
        qFatal() << "Fatal: SinglePlayerInGameUI::advance current level not single player level or empty";
    int downcount = currentLevel->getDowncount();
    int score = currentLevel->getScore();
    ui.timeValueLabel->setText(QString::number(downcount));
    ui.scoreValueLabel->setText(QString::number(score));
    update();
}


