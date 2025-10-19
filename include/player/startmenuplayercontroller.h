#pragma once
#ifndef STARTMENUPLAYERCONTROLLER_H
#define STARTMENUPLAYERCONTROLLER_H

#include "playercontroller.h"

class StartMenuPlayerController : public PlayerController
{
    Q_OBJECT
public:
    StartMenuPlayerController(MainWindow*);
public:
    virtual void enterLevelInitialization() override;
};

#endif // STARTMENUPLAYERCONTROLLER_H
