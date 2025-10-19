#ifndef CHARACTER_H
#define CHARACTER_H

#include "actor.h"

class PlayerController;

class Character : public Actor
{
    Q_OBJECT
    friend class PlayerController;
public:
    Character(Actor*);
protected:
    PlayerController* controller;
public:
    inline PlayerController* getController() { return controller; }
};

#endif // CHARACTER_H
