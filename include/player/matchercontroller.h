#pragma once
#ifndef MATCHERCONTROLLER_H
#define MATCHERCONTROLLER_H

#include "playercontroller.h"

class Matcher;
class TileLevel;
enum class GameEndCause;
enum class PropType;

class MatcherController : public PlayerController
{
    Q_OBJECT
    friend class Matcher;
    friend class TileLevel;
public:
    MatcherController(MainWindow*);
public:
    virtual void enterLevelInitialization() override;
protected:
    virtual void showInGameUI() = 0;
    virtual void showGameEndUI(GameEndCause) = 0;
    virtual void showGamePauseUI(bool);
protected:
    virtual void onKeyPressed(QKeyEvent*) override;
    virtual void onKeyReleased(QKeyEvent*) override;
protected:
    bool gameEnded = false;
    virtual void gameEnd(GameEndCause);
    bool gamePaused = false;
    virtual void gamePause(bool);
protected:
    void applyPropEffect(PropType, Matcher*);
};

#endif // MATCHERCONTROLLER_H
