#pragma once
#ifndef LEVEL_H
#define LEVEL_H

#include <QGraphicsScene>
#include "mainwindow.h"

class PlayerController;
class Character;
class EmptyGraphicsItem;

// 代表游戏内的场景的基类，游戏内一共有三个场景：开始界面、单人模式、双人模式
class Level : public QGraphicsScene
{
    Q_OBJECT
    friend class MainWindow;
public:
    Level(MainWindow* window);
    ~Level();
protected:
    // 表示场景中将要被控制器操控的一组角色以及将要被绑定到控制器 view 上的相机锚点（初始时 view 中心正对 cameraPivot）
    QList<Character*> puppets;
    EmptyGraphicsItem* cameraPivot;
    // 场景中的角色控制器
    PlayerController* controller;
    // 场景中所有 item 的 root parent
    EmptyGraphicsItem* root;
public:
    // root 可能会被改变，加 const 只限制指针没有意义。实际上把 root 改成 public 更好。
    inline EmptyGraphicsItem* getRoot() { return root; }
public:
    // 进入新关卡时的触发（由 MainWindow 触发）自定义的初始化，根据不同的关卡和不同的控制器（二者通常对应匹配）而不同。和 PlayerController 的 enterLevelInitialization 是配套的。
    virtual void levelInitialization();
protected:
    void advance();
    void advanceCall();
    virtual void customAdvance();
};
#endif // LEVEL_H
