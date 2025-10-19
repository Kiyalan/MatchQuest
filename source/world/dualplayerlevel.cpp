#include "dualplayerlevel.h"
#include "dualmatchercontroller.h"
#include "matcher.h"
#include "prop.h"
#include "propgenerator.h"

DualPlayerLevel::DualPlayerLevel(MainWindow *window, int row, int col)
    : TileLevel(window, row, col)
{
    qDebug() << "Process: dual player level construction";
    placeMatcher();
}

void DualPlayerLevel::placeMatcher()
{
    static const QString player1AvatarPath = ":/image/Lamball.png";
    static const QString player2AvatarPath = ":/image/Cattiva.png";
    QRectF rect1 = generateRandomStartPoint();
    QRectF rect2 = generateRandomStartPoint();
    Matcher* player1 = new Matcher(root, player1AvatarPath);
    Matcher* player2 = new Matcher(root, player2AvatarPath);
    player1->setPos(rect1.center());
    player2->setPos(rect2.center());
    puppets.append(player1);
    puppets.append(player2);
}

void DualPlayerLevel::levelInitialization()
{
    qDebug() << "Process: dual player level init";
    TileLevel::levelInitialization();
    propGen = new PropGenerator({}, PropGenerator::defaultGenerationDeltaTime, root);
    // 必须在 propGen 之后检测。
    checkNoSolution(puppets[0]);
    checkNoSolution(puppets[1]);
}

void DualPlayerLevel::tileEliminationResponse(Character* trigger)
{
    qDebug() << "Process: DualPlayerLevel tile elimination response";
    if (trigger == puppets[0])
        score[0] += 2;
    if (trigger == puppets[1])
        score[1] += 2;
    qDebug() << "    Detail: currentTileCount " << currentTileCount;
    if (currentTileCount == 0)
        gameEnd(GameEndCause::Completed);
    ;
}

void DualPlayerLevel::downcountZeroResponse()
{
    gameEnd(GameEndCause::Timeout);
}

void DualPlayerLevel::noSolutionResponse(Character*)
{
    gameEnd(GameEndCause::PlayerTrapped);
}

void DualPlayerLevel::gameEnd(GameEndCause cause)
{
    qDebug() << "Process: dual player level game end in with" << gameEndCauseToString(cause);
    TileLevel::gameEnd(cause);
    auto player = static_cast<DualMatcherController*>(controller);
    if (!player)
        qFatal() << "Fatal: dualPlayerLevel::gameEnd player is null";
    player->gameEnd(cause);
    qDebug() << "Process: dual player level game end out";
}

