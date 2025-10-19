#include "singlematchercontroller.h"
#include "matcher.h"
#include "singleplayerlevel.h"
#include "singleplayeringameui.h"
#include "singleplayergameendui.h"
#include "prop.h"
#include <QKeyEvent>

SingleMatcherController::SingleMatcherController(MainWindow* window)
    : MatcherController(window)
{
    qDebug() << "Process: single matcher controller construction";
}

void SingleMatcherController::enterLevelInitialization()
{
    MatcherController::enterLevelInitialization();
    player = static_cast<Matcher*>(puppets[0]);
}

void SingleMatcherController::onKeyPressed(QKeyEvent* event)
{
    if (gameEnded)
        return;
    if (event->isAutoRepeat())
        return;
    qDebug() << "Process: single matcher controller receive key pressed event " << event->key();
    switch (event->key()) {
    case Qt::Key::Key_W:
        if (!gamePaused) player->moveUp(1);
        break;
    case Qt::Key::Key_S:
        if (!gamePaused) player->moveUp(-1);
        break;
    case Qt::Key::Key_A:
        if (!gamePaused) player->moveRight(-1);
        break;
    case Qt::Key::Key_D:
        if (!gamePaused) player->moveRight(1);
        break;
    default:
        qDebug() << "    Detail: single matcher controller forward key pressed event to matcher controller ";
        MatcherController::onKeyPressed(event);
    }
}
void SingleMatcherController::onKeyReleased(QKeyEvent* event)
{
    if (gameEnded)
        return;
    if (event->isAutoRepeat())
        return;
    qDebug() << "Process: single matcher controller receive key released event " << event->key();
    switch (event->key()) {
    case Qt::Key::Key_W:
        if (!gamePaused) player->moveUp(-1);
        break;
    case Qt::Key::Key_S:
        if (!gamePaused) player->moveUp(1);
        break;
    case Qt::Key::Key_A:
        if (!gamePaused) player->moveRight(1);
        break;
    case Qt::Key::Key_D:
        if (!gamePaused) player->moveRight(-1);
        break;
    default:
        MatcherController::onKeyReleased(event);
    }
}

void SingleMatcherController::showInGameUI()
{
    auto inGameUI = new SinglePlayerInGameUI(this, view);
    showUserInterface(inGameUI);
}

void SingleMatcherController::showGameEndUI(GameEndCause cause)
{
    qDebug() << "Process: single matcher controller show game end ui with " << gameEndCauseToString(cause);
    auto gameEndUI = new SinglePlayerGameEndUI(cause, this, view);
    showUserInterface(gameEndUI);
}

void SingleMatcherController::gameEnd(GameEndCause cause)
{
    qDebug() << "Process: single matcher controller game end with " << gameEndCauseToString(cause);
    MatcherController::gameEnd(cause);
    player->stopMoving();
}
