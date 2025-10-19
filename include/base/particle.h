#pragma once
#ifndef PARTICLE_H
#define PARTICLE_H
#include "actor.h"
#include <QTimeLine>

class Particle : public Actor
{
    Q_OBJECT
public:
    Particle(QGraphicsObject*, int duration);
    ~Particle();
protected:
    QTimeLine lifeClock;
protected:
    virtual void pause(bool) override;
public:
    void manualRemoval();
public slots:
    void timedRemoval();

};

#endif // PARTICLE_H
