#pragma once
#ifndef MATCHER_H
#define MATCHER_H

#include "character.h"

class Tile;
class MatcherController;


class Matcher : public Character
{
    friend MatcherController;
    Q_OBJECT
public:
    Matcher(Actor*, QString);
public:
    static constexpr qreal matcherMovingSpeed = 187.5; // logic pixels per seconds
    static constexpr qreal matcherSize = 60.; // logic pixels
protected:
    QPointF velocity {0, 0};
protected:
    QPixmap avatar;
public:
    void paint(QPainter *, const QStyleOptionGraphicsItem *, QWidget *widget = nullptr) override;
public:
    QRectF boundingRect() const override;
    QPainterPath shape() const override;
// public:
//     QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;
public:
    void advance(int) override;
public:
    void moveUp(int);
    void moveRight(int);
    void stopMoving();
private:
    Tile *lastTouchTile = nullptr;
};

#endif // MATCHER_H
