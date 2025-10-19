#include "hinter.h"
#include "tilelevel.h"
#include "configuration.h"
#include "matcher.h"
#include "tile.h"
#include "tileelimination.h"

const QPen Hinter::defaultHintPen = QPen(/*TileElimination::defaultPen.brush()*/QColor(0xEE92C2), TileElimination::defaultPen.widthF(), Qt::PenStyle::DashLine);

Hinter::Hinter(Matcher *trigger, int duration, Actor* parent)
    : EmptyGraphicsItem(parent), triggerPlayer(trigger), updateTimer(duration, this), lifeDuration(duration)/*, restLifeDuration(duration)*/
{
    connect(&updateTimer, &QTimeLine::valueChanged, this, &Hinter::updateHint);
    connect(&updateTimer, &QTimeLine::finished, this, &Hinter::timedHintEnd);
    updateTimer.start();
    // connect(&lifeClock, &QTimer::timeout, this, &Hinter::timedHintEnd);
    // lifeClock.start(duration);
}
void Hinter::updateHint()
{
    // qDebug() << "Process: hinter update hint";
    TileLevel* tileLevel = static_cast<TileLevel*>(triggerPlayer->scene());
    if (HintedPath[0].x() != -1 && tileLevel->map[tileLevel->gridToLinear(HintedPath[0])] != nullptr && tileLevel->map[tileLevel->gridToLinear(HintedPath[0])] != nullptr) {
            ;
    } else {
        QList<QPoint> reach = tileLevel->getReachableTiles(triggerPlayer);
        const int colorCount = static_cast<size_t>(Tile::TileType::TileTypeCount);
        QList<QPoint> colorBucket[colorCount];
        for (auto &x : reach) {
            int color = static_cast<int>(static_cast<Tile*>(tileLevel->map[tileLevel->gridToLinear(x)])->getTileType());
            colorBucket[color].append(x);
        }
        for (int i = 0; i < colorCount; ++i)
            for (int l = 0; l < colorBucket[i].size(); ++l)
                for (int r = l + 1; r < colorBucket[i].size(); ++r) {
                    if (tileLevel->testTileEliminationTwoPass(colorBucket[i][l], colorBucket[i][r], HintedPath[1], HintedPath[2])){
                        HintedPath[0] = colorBucket[i][l], HintedPath[3] = colorBucket[i][r];
                        if (hintParticle)
                            hintParticle->manualRemoval();
                        hintParticle = new TileElimination(HintedPath, defaultHintDuration, defaultHintPen, tileLevel->getRoot());
                        hintParticle->setZValue(1);
                    }
                }
    }
    /*restLifeDuration -= Configuration::milisecondsPerFrame;
    qDebug() << "    Detail: rest life: " << restLifeDuration;
    if (restLifeDuration < Configuration::milisecondsPerFrame)
        timedHintEnd();*/
}

void Hinter::timedHintEnd()
{
    qDebug() << "Process: hinter timed end";
    updateTimer.stop();
    if (hintParticle)
        hintParticle->manualRemoval();
    scene()->removeItem(this);
    deleteLater();
}

void Hinter::manualHintEnd()
{
    updateTimer.stop();
    if (hintParticle)
        hintParticle->manualRemoval();
    scene()->removeItem(this);
    deleteLater();
}
void Hinter::pause(bool p)
{
    qDebug() << "Process: hinter pause " << p;
    if (p && !paused) {
        updateTimer.stop();
        qDebug() << "    Detail: current time" << updateTimer.currentTime();
    } else if (!p && paused) {
        updateTimer.resume();
        qDebug() << "    Detail: current time" << updateTimer.currentTime();
    }
    Actor::pause(p);
}
