#pragma once
#ifndef DUALPLAYERGAMEENDUI_H
#define DUALPLAYERGAMEENDUI_H

#include "userinterface.h"
#include "../../resource/ui/ui_dualplayergameendui.h"

enum class GameEndCause;

class DualPlayerGameEndUI : public UserInterface
{
    Q_OBJECT
public:
    explicit DualPlayerGameEndUI(GameEndCause, PlayerController *, QWidget *);
protected:
    void advance() override;
private:
    Ui::DualPlayerGameEndUI ui;
};

#endif // DUALPLAYERGAMEENDUI_H
