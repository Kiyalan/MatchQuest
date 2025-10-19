#include "particle.h"
#include <QTimer>
#include <QGraphicsScene>

Particle::Particle(QGraphicsObject* parent, int duration)
    : Actor(parent), lifeClock(duration, this)
{
    connect(&lifeClock, &QTimeLine::finished, this, &Particle::timedRemoval);
    lifeClock.start();
}

Particle::~Particle()
{

}

void Particle::timedRemoval()
{
    scene()->removeItem(this);
    deleteLater();
}

void Particle::manualRemoval()
{
    lifeClock.stop();
    scene()->removeItem(this);
    deleteLater();
}

void Particle::pause(bool p)
{
    if (p && !paused) {
        lifeClock.stop();
    } else if (!p && paused) {
        lifeClock.resume();
    }
    Actor::pause(p);
}
