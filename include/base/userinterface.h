#pragma once
#ifndef USERINTERFACE_H
#define USERINTERFACE_H

#include <QWidget>

class PlayerController;

class UserInterface : public QWidget
{
    friend class PlayerController;
    Q_OBJECT
public:
    explicit UserInterface(PlayerController *, QWidget *parent = nullptr);
protected:
    PlayerController *controller;
protected:
    virtual void advance();
signals:
};

#endif // USERINTERFACE_H
