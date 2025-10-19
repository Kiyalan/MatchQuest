#include "userinterface.h"
#include "playercontroller.h"
UserInterface::UserInterface(PlayerController *control, QWidget *parent)
    : QWidget{parent}, controller(control)
{

}

void UserInterface::advance()
{

}
