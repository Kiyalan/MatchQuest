#pragma once
#ifndef SINGLEPLAYERGAMEENDUI_H
#define SINGLEPLAYERGAMEENDUI_H

#include "userinterface.h"
#include "../../resource/ui/ui_singleplayergameendui.h"

enum class GameEndCause;

class SinglePlayerGameEndUI : public UserInterface
{
    Q_OBJECT
public:
    explicit SinglePlayerGameEndUI(GameEndCause, PlayerController *, QWidget *);
protected:
    void advance() override;
private:
    Ui::SinglePlayerGameEndUI ui;
};

#endif // SINGLEPLAYERGAMEENDUI_H
