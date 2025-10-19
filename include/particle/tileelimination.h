#pragma once
#ifndef TILEELIMINATION_H
#define TILEELIMINATION_H

#include "particle.h"
#include <QPen>

class TileElimination : public Particle
{
    Q_OBJECT
public:
    static const int defaultDuration;
    static const QPen defaultPen;
public:
    TileElimination(QPoint position[4], int duration, QPen connectPen, Actor* parent);
    ~TileElimination();
protected:
    QPointF node[4];
    QRectF bounding;
    QPen drawPen;
    void paint(QPainter *, const QStyleOptionGraphicsItem *, QWidget *) override;
    QRectF boundingRect() const override;
};


#endif
