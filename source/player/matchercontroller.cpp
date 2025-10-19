#include "matchercontroller.h"
// #include "singlematchercontroller.h"
// #include "dualmatchercontroller.h"
#include "tilelevel.h"
#include "matcher.h"
#include "prop.h"
#include "hinter.h"
#include "actor.h"
#include "singleplayerpausegameui.h"
#include <QKeyEvent>

MatcherController::MatcherController(MainWindow* window)
    : PlayerController(window)
{

}

void MatcherController::enterLevelInitialization()
{
    PlayerController::enterLevelInitialization();
    showInGameUI();
}

void MatcherController::showGamePauseUI(bool p)
{
    qDebug() << "Process: matcher controller show game pause ui " << p;
    static SinglePlayerPauseGameUI* gamePauseUI = nullptr;
    if (p) {
        gamePauseUI = new SinglePlayerPauseGameUI(this, view);
        showUserInterface(gamePauseUI);
    } else {
        hudList.removeAll(gamePauseUI);
        delete gamePauseUI;
    }
}

void MatcherController::gameEnd(GameEndCause cause)
{
    qDebug() << "Process: Matcher controller game end with " << gameEndCauseToString(cause);
    gameEnded = true;
    showGameEndUI(cause);
}

void MatcherController::gamePause(bool p)
{
    if (!gamePaused && p) {
        showGamePauseUI(p);
    } else if (gamePaused && !p) {
        showGamePauseUI(p);
    }
    gamePaused = p;
}

void MatcherController::onKeyPressed(QKeyEvent* event)
{
    if (gameEnded)
        return;
    if (event->isAutoRepeat())
        return;
    qDebug() << "Process: single matcher controller receive key pressed event " << event->key();
    auto tileLevel = static_cast<TileLevel*>(level);
    if (!tileLevel)
        qFatal() << "Fatal: MatcherController::onKeyPressed tileLevel empty";
    switch (event->key()) {
    case Qt::Key::Key_Escape:
        tileLevel->gamePause(!gamePaused);
        break;
    }
}
void MatcherController::onKeyReleased(QKeyEvent*)
{

}

void MatcherController::applyPropEffect(Prop::PropType type, Matcher* player)
{
    qDebug() << "Process: matcher player apply prop effect" << Prop::propImagePathTable[static_cast<int>(type)] << " to " << player;
    auto tileLevel = static_cast<TileLevel*>(level);
    // auto singleControl = static_cast<SingleMatcherController*>(player->getController());
    // auto dualControl = static_cast<DualMatcherController*>(player->getController());
    switch(type)
    {
    case Prop::PropType::TimeProlong:
        tileLevel->downcount += 30;
        break;
    case Prop::PropType::Shuffle:
        player->lastTouchTile = nullptr;
        tileLevel->mapShuffle();
        tileLevel->checkNoSolution(player);
        break;
    case Prop::PropType::Hint: {
        auto hinter = new Hinter(player, 10000, tileLevel->getRoot());
    }
        break;
    case Prop::PropType::Flash:
        break;
    case Prop::PropType::Dizzy:
        break;
    case Prop::PropType::Freeze:
        break;
    }
    qDebug() << "Process: matcher player prop effect applied";
}
