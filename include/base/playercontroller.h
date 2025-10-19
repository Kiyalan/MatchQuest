#pragma once
#ifndef PLAYERCONTROLLER_H
#define PLAYERCONTROLLER_H

#include <QObject>
#include <QGraphicsView>

class MainWindow;
class Level;
class Character;
class UserInterface;
class EmptyGraphicsItem;

class PlayerController : public QObject
{
    Q_OBJECT
    friend class MainWindow;
    friend class Level;
    friend class SinglePlayerPauseGameUI;
public:
    PlayerController(MainWindow*);
    ~PlayerController();
protected:
    Level* level = nullptr;
    QList<Character*> puppets;
    EmptyGraphicsItem* cameraPivot;
public:
    inline Level* getLevel() const { return level; }
protected:
    static const int clientAreaWidth = 1980, clientAreaHeight = 1080;
    MainWindow* window = nullptr;
    int viewWidth = 1980, viewHeight = 1080;
    QGraphicsView* view = nullptr;
public:
    // 不能加 const 因为打开关卡并不是 const 函数
    inline MainWindow* getWindow() { return window; }
    inline QSize getViewSize() const { return QSize(viewWidth, viewHeight); }
protected:
    // 进入新关卡时的触发（由 MainWindow 触发）自定义的初始化，根据不同的关卡和不同的控制器（二者通常对应匹配）而不同
    virtual void enterLevelInitialization();
protected:
    // 由 MainWindow 转发的输入事件
    virtual void onKeyPressed(QKeyEvent*);
    virtual void onKeyReleased(QKeyEvent*);
protected:
    // 由 Level 转发的逐帧更新事件。这里包括一个可以在子类重写的一个虚函数。advance 会调用 customAdvance。
    void advance();
    virtual void customAdvance();
    // 逐帧更新 view 的映射区域中心到 cameraPivot 的位置
    void updateViewPosition();
protected:
    // UI 管理
    QList<UserInterface*> hudList;
    void showUserInterface(UserInterface*);
signals:
    // void requestOpenLevel(StartMenuLevel*);
};

#endif // PLAYERCONTROLLER_H
