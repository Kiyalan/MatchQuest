#include "startmenulevel.h"
#include "playercontroller.h"
#include "startmenuui.h"

StartMenuLevel::StartMenuLevel(MainWindow *window)
    : Level(window)
{

}

void StartMenuLevel::levelInitialization()
{
    Level::levelInitialization();
    qDebug() << "Process: start menu level level initialization";
}

void StartMenuLevel::customAdvance()
{
    Level::customAdvance();
}
