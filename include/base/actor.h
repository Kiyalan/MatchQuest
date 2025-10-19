#pragma once
#ifndef ACTOR_H
#define ACTOR_H

#include <QGraphicsObject>

class Actor : public QGraphicsObject
{
    Q_OBJECT
public:
    Actor(QGraphicsObject* parent = nullptr) : QGraphicsObject(parent) {}
protected:
    bool paused = false;
public:
    virtual void pause(bool p);
};

#endif // ACTOR_H
