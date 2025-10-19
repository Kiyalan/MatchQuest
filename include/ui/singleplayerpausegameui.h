#pragma once
#ifndef SINGLEPLAYERPAUSEGAMEUI_H
#define SINGLEPLAYERPAUSEGAMEUI_H

#include "userinterface.h"
#include "../../resource/ui/ui_singleplayerpausegameui.h"

enum class GameEndCause;

class SinglePlayerPauseGameUI : public UserInterface
{
    Q_OBJECT
public:
    explicit SinglePlayerPauseGameUI(PlayerController *, QWidget *);
protected:
    void advance() override;
    void saveData();
private:
    Ui::SinglePlayerPauseGameUI ui;
};

#endif // SINGLEPLAYERGAMEENDUI_H
