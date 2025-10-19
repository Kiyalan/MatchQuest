#pragma once
#ifndef EMPTYGRAPHICSITEM_H
#define EMPTYGRAPHICSITEM_H

#include "actor.h"

class EmptyGraphicsItem : public Actor
{
    Q_OBJECT
public:
    EmptyGraphicsItem(Actor* parent = nullptr);
    QRectF boundingRect() const override;
    void paint(QPainter *, const QStyleOptionGraphicsItem *, QWidget *) override;
};

#endif // EMPTYGRAPHICSITEM_H
