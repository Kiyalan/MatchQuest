#pragma once
#ifndef DUALPLAYERINGAMEUI_H
#define DUALPLAYERINGAMEUI_H
#include "userinterface.h"
#include "../../resource/ui/ui_dualplayeringameui.h"

class DualPlayerInGameUI : public UserInterface
{
    Q_OBJECT
public:
    explicit DualPlayerInGameUI(PlayerController *, QWidget *);
protected:
    void advance() override;
private:
    Ui::DualPlayerInGameUI ui;
};

#endif // DUALPLAYERINGAMEUI_H
