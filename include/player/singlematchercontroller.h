#pragma once
#ifndef SINGLEMATCHERCONTROLLER_H
#define SINGLEMATCHERCONTROLLER_H

#include "matchercontroller.h"

class Matcher;
class TileLevel;
enum class GameEndCause;
enum class PropType;

class SingleMatcherController : public MatcherController
{
    Q_OBJECT
    // 单人关卡是单人控制器的友元
    friend class SinglePlayerLevel;
    // Serialization 为外部序列化工具，所以友元从方便性来看是必要的
    friend class Serialization;

    // 构造函数
public:
    SingleMatcherController(MainWindow*);

    // 控制的角色
private:
    Matcher* player = nullptr;
public:
    inline const Matcher* getPlayer() const { return player; }
    inline Matcher* getPlayer() { return player; }

    // 输入事件
protected:
    void onKeyPressed(QKeyEvent*) override;
    void onKeyReleased(QKeyEvent*) override;

    // UI 显示
protected:
    void showInGameUI() override;
    void showGameEndUI(GameEndCause) override;

    // 对象流程控制
protected:
    // 与关卡绑定之后的初始化
    void enterLevelInitialization() override;
protected:
    // 游戏结束
    void gameEnd(GameEndCause) override;
};

#endif // SINGLEMATCHERCONTROLLER_H
