#include "startmenuplayercontroller.h"
#include "startmenuui.h"


StartMenuPlayerController::StartMenuPlayerController(MainWindow *window)
    : PlayerController(window)
{

}

void StartMenuPlayerController::enterLevelInitialization()
{
    qDebug() << "Process: start menu player controller enter level init";
    PlayerController::enterLevelInitialization();
    qDebug() << "Process: start menu player controller level initialization";
    auto ui = new StartMenuUI(this, view);
    showUserInterface(ui);
}
