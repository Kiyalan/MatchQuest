#pragma once
#ifndef HINTER_H
#define HINTER_H

#include "emptygraphicsitem.h"
#include <QTimeLine>

class Matcher;
class Tile;
class TileElimination;

class Hinter : public EmptyGraphicsItem
{
    Q_OBJECT
public:
    static const int defaultHintDuration = 10000; // in milliseconds
    static const QPen defaultHintPen;
    Hinter(Matcher *trigger, int duration = defaultHintDuration, Actor* parent = nullptr);
protected:
    Matcher *triggerPlayer = nullptr;
    QPoint HintedPath[4] {QPoint(-1, -1), QPoint(-1, -1), QPoint(-1, -1), QPoint(-1, -1)};
    TileElimination *hintParticle = nullptr;
protected:
    // QTimer lifeClock;
    QTimeLine updateTimer;
    int lifeDuration;
    int restLifeDuration;
protected:
    void timedHintEnd();
public:
    void manualHintEnd();
protected slots:
    void updateHint();
protected:
    void pause(bool) override;
};

#endif // HINTER_H
