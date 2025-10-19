#include "dualmatchercontroller.h"
#include "matcher.h"
#include "dualplayerlevel.h"
#include "dualplayeringameui.h"
#include "dualplayergameendui.h"
#include <QKeyEvent>

DualMatcherController::DualMatcherController(MainWindow* window)
    : MatcherController(window)
{

}

void DualMatcherController::enterLevelInitialization()
{
    MatcherController::enterLevelInitialization();
    player[0] = static_cast<Matcher*>(puppets[0]);
    player[1] = static_cast<Matcher*>(puppets[1]);
}

void DualMatcherController::onKeyPressed(QKeyEvent* event)
{
    if (gameEnded)
        return;
    if (event->isAutoRepeat())
        return;
    qDebug() << "Process: dual matcher controller receive key pressed event " << event->key();
    switch (event->key()) {
    case Qt::Key::Key_W:
        if (!gamePaused) player[0]->moveUp(1);
        break;
    case Qt::Key::Key_S:
        if (!gamePaused) player[0]->moveUp(-1);
        break;
    case Qt::Key::Key_A:
        if (!gamePaused) player[0]->moveRight(-1);
        break;
    case Qt::Key::Key_D:
        if (!gamePaused) player[0]->moveRight(1);
        break;
    case Qt::Key::Key_Up:
        if (!gamePaused) player[1]->moveUp(1);
        break;
    case Qt::Key::Key_Down:
        if (!gamePaused) player[1]->moveUp(-1);
        break;
    case Qt::Key::Key_Left:
        if (!gamePaused) player[1]->moveRight(-1);
        break;
    case Qt::Key::Key_Right:
        if (!gamePaused) player[1]->moveRight(1);
        break;
    default:
        qDebug() << "    Detail: dual matcher controller forward key pressed event to matcher controller ";
        MatcherController::onKeyPressed(event);
    }
}

void DualMatcherController::onKeyReleased(QKeyEvent* event)
{
    if (gameEnded)
        return;
    if (event->isAutoRepeat())
        return;
    qDebug() << "Process: dual matcher controller receive key pressed event " << event->key();
    switch (event->key()) {
    case Qt::Key::Key_W:
        if (!gamePaused) player[0]->moveUp(-1);
        break;
    case Qt::Key::Key_S:
        if (!gamePaused) player[0]->moveUp(1);
        break;
    case Qt::Key::Key_A:
        if (!gamePaused) player[0]->moveRight(1);
        break;
    case Qt::Key::Key_D:
        if (!gamePaused) player[0]->moveRight(-1);
        break;
    case Qt::Key::Key_Up:
        if (!gamePaused) player[1]->moveUp(-1);
        break;
    case Qt::Key::Key_Down:
        if (!gamePaused) player[1]->moveUp(1);
        break;
    case Qt::Key::Key_Left:
        if (!gamePaused) player[1]->moveRight(1);
        break;
    case Qt::Key::Key_Right:
        if (!gamePaused) player[1]->moveRight(-1);
        break;
    default:
        qDebug() << "    Detail: dual matcher controller forward key pressed event to matcher controller ";
        MatcherController::onKeyReleased(event);
    }
}

void DualMatcherController::showInGameUI()
{
    auto inGameUI = new DualPlayerInGameUI(this, view);
    showUserInterface(inGameUI);
}

void DualMatcherController::gameEnd(GameEndCause cause)
{
    MatcherController::gameEnd(cause);
    player[0]->stopMoving();
    player[1]->stopMoving();
}

void DualMatcherController::showGameEndUI(GameEndCause cause)
{
    auto gameEndUI = new DualPlayerGameEndUI(cause, this, view);
    showUserInterface(gameEndUI);
}
