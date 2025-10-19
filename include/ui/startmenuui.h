#pragma once
#ifndef STARTMENUUI_H
#define STARTMENUUI_H

#include "userinterface.h"
#include "../../resource/ui/ui_startmenuui.h"

class StartMenuUI : public UserInterface
{
    Q_OBJECT
public:
    StartMenuUI(PlayerController*, QWidget*);
protected:
    void advance() override;
    int mapWidth, mapHeight;
// protected slots:
    // void loadData();
private:
    Ui::StartMenuUI ui;
};

#endif // STARTMENUUI_H
