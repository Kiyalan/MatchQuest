#pragma once
#ifndef DUALMATCHERCONTROLLER_H
#define DUALMATCHERCONTROLLER_H

#include "matchercontroller.h"

class Matcher;
class TileLevel;
enum class GameEndCause;

class DualMatcherController : public MatcherController
{
    Q_OBJECT
    friend class DualPlayerLevel;
    // Serialization 为外部序列化工具，所以友元从方便性来看是必要的
    friend class Serialization;
public:
    DualMatcherController(MainWindow*);
public:
    void enterLevelInitialization() override;
protected:
    Matcher *player[2] {nullptr, nullptr};
    void onKeyPressed(QKeyEvent*) override;
    void onKeyReleased(QKeyEvent*) override;
protected:
    void showInGameUI() override;
    void showGameEndUI(GameEndCause) override;
protected:
    bool gameEnded = false;
    void gameEnd(GameEndCause) override;
};

#endif // DUALMATCHERCONTROLLER_H
