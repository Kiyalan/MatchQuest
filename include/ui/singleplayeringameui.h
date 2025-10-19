#pragma once
#ifndef SINGLEPLAYERINGAMEUI_H
#define SINGLEPLAYERINGAMEUI_H

#include "userinterface.h"
#include "../../resource/ui/ui_singleplayeringameui.h"

class SinglePlayerInGameUI : public UserInterface
{
    Q_OBJECT
public:
    explicit SinglePlayerInGameUI(PlayerController *, QWidget *);
protected:
    void advance() override;
private:
    Ui::SinglePlayerInGameUI ui;
};

#endif // SINGLEPLAYERINGAMEUI_H
