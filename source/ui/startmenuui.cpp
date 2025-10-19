#include "startmenuui.h"
#include "playercontroller.h"
#include "mainwindow.h"

StartMenuUI::StartMenuUI(PlayerController* control, QWidget *parent)
    : UserInterface(control, parent)
{
    qDebug() << "Process start menu ui construction";
    ui.setupUi(this);
    if (!connect(ui.singlePlayerButton, &QPushButton::clicked, MainWindow::getInstance(), &MainWindow::openSinglePlayerLevel))
        qFatal() << "Process: start menu ui establish connect with window single player";
    if (!connect(ui.dualPlayerButton, &QPushButton::clicked, MainWindow::getInstance(), &MainWindow::openDualPlayerLevel))
        qFatal() << "Process: start menu ui establish connect with window dual player";
    connect(ui.quitButton, &QPushButton::clicked, nullptr, &QCoreApplication::quit);
    connect(ui.loadButton, &QPushButton::clicked, MainWindow::getInstance(), &MainWindow::loadLevelFromFile);
}

void StartMenuUI::advance()
{
    // qDebug() << "Process: start menu ui advance " << ui.widthSpinBox->value() << " " << ui.heightSpinBox->value();
    MainWindow::getInstance()->inputWindowWidth = mapWidth = ui.widthSpinBox->value();
    MainWindow::getInstance()->inputWindowHeight = mapHeight = ui.heightSpinBox->value();
    MainWindow::getInstance()->inputLevelPath = ui.pathEditor->toPlainText();
}
