#include "singleplayergameendui.h"
#include "singleplayerlevel.h"
#include "playercontroller.h"

SinglePlayerGameEndUI::SinglePlayerGameEndUI(GameEndCause cause, PlayerController *control, QWidget *parent)
    : UserInterface(control, parent)
{
    qDebug() << "Process: single player game end ui construction";
    ui.setupUi(this);
    // 初始化结束原因、最终分数、剩余时间
    ui.gameEndCauseLabel->setText(gameEndCauseToString(cause));
    auto singleLevel = static_cast<SinglePlayerLevel*>(control->getLevel());
    if (singleLevel == nullptr)
        qFatal() << "Fatal: SinglePlayerGameEndUI::SinglePlayerGameEndUI control->getLevel() empty or not single player level";
    ui.scoreValueLabel->setText(QString::number(singleLevel->getScore()));
    ui.restTimeValueLabel->setText(QString::number(singleLevel->getDowncount()));
    connect(ui.startMenuButton, &QPushButton::clicked, this, [this](bool)
    {
        auto window = controller->getWindow();
        if (!window)
            qFatal() << "Fatal SinglePlayerGameEndUI::SinglePlayerGameEndUI window empty";
        window->MainWindow::openStartMenuLevel();
    });
    update();
}

void SinglePlayerGameEndUI::advance()
{

}
