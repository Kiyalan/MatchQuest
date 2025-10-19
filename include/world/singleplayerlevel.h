#ifndef SINGLEPLAYERLEVEL_H
#define SINGLEPLAYERLEVEL_H

#include "tilelevel.h"

class SinglePlayerLevel : public TileLevel
{
    Q_OBJECT
    // 单人控制器是单人关卡的友元
    friend class SingleMatcherController;
    // Serialization 为外部序列化工具，所以友元从方便性来看是必要的
    friend class Serialization;

    // 构造函数，接受初始的长和宽
public:
    SinglePlayerLevel(MainWindow *, int, int);

    // 关卡流程控制
protected:
    // 在与角色控制器绑定之后的初始化
    void levelInitialization() override;
protected:
    // 角色的初始防放置
    void placeMatcher() override;
protected:
    // 一些特化的流程响应，包括计时归零、检测到无解、方块消除
    void downcountZeroResponse() override;
    void noSolutionResponse(Character*) override;
    void tileEliminationResponse(Character*) override;
protected:
    // 游戏结束
    void gameEnd(GameEndCause) override;

    // 分数
protected:
    int score = 0;
public:
    int getScore() const { return score; }
};

#endif // SINGLEPLAYERLEVEL_H
