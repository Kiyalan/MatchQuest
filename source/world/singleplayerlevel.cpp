#include "singleplayerlevel.h"
#include "matcher.h"
#include "singlematchercontroller.h"
#include "emptygraphicsitem.h"
#include "propgenerator.h"

SinglePlayerLevel::SinglePlayerLevel(MainWindow *window, int row, int col)
    : TileLevel(window, row, col)
{
    qDebug() << "Process: single player level construction";
    placeMatcher();
}

void SinglePlayerLevel::placeMatcher()
{
    QRectF rect = generateRandomStartPoint();
    static const QString playerAvatarPath = ":/image/Lamball.png";
    Matcher* player = new Matcher(root, playerAvatarPath);
    player->setPos(rect.center());
    puppets.append(player);
}

void SinglePlayerLevel::levelInitialization()
{
    qDebug() << "Process: single player level init";
    TileLevel::levelInitialization();
    propGen = new PropGenerator({
        Prop::PropType::TimeProlong,
        Prop::PropType::Shuffle,
        Prop::PropType::Hint,
        Prop::PropType::Flash
    }, PropGenerator::defaultGenerationDeltaTime, root);
    // 必须在 propGen 生成之后之后检测，否则 propGen 未生成就会被触发删除。
    checkNoSolution(puppets[0]);
}

void SinglePlayerLevel::downcountZeroResponse()
{
    gameEnd(GameEndCause::Timeout);
}

void SinglePlayerLevel::noSolutionResponse(Character*)
{
    gameEnd(GameEndCause::PlayerTrapped);
}

void SinglePlayerLevel::tileEliminationResponse(Character* trigger)
{
    qDebug() << "Process: SinglePlayerLevel tile elimination response";
    if (trigger == puppets[0])
        score += 2;
    qDebug() << "    Detail: currentTileCount " << currentTileCount;
    if (currentTileCount == 0)
        gameEnd(GameEndCause::Completed);
        ;

}

void SinglePlayerLevel::gameEnd(GameEndCause cause)
{
    qDebug() << "Process: single player level game end in with" << gameEndCauseToString(cause);
    TileLevel::gameEnd(cause);
    auto player = static_cast<SingleMatcherController*>(controller);
    if (!player)
        qFatal() << "Fatal: SinglePlayerLevel::gameEnd player is null";
    player->gameEnd(cause);
    qDebug() << "Process: single player level game end out";
}

