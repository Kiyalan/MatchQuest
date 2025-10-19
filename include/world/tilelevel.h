#pragma once
#ifndef TILELEVEL_H
#define TILELEVEL_H

#include "level.h"
#include <QTimeLine>

class Matcher;
class Tile;
class Prop;
class PropGenerator;
class Actor;


// 游戏结束原因枚举
enum class GameEndCause
{
    PlayerTrapped,
    Timeout,
    Completed
};
inline QString gameEndCauseToString(GameEndCause cause)
{
    switch(cause) {
        case GameEndCause::PlayerTrapped:
            return QString("Player trapped");
        case GameEndCause::Timeout:
            return QString("Timeout");
        case GameEndCause::Completed:
            return QString("Puzzle completed");
    }
    return QString("Puzzle completed");
}


class TileLevel : public Level
{
    Q_OBJECT
    friend class Hinter;
    friend class PropGenerator;
    friend class MatcherController;
    friend class SinglePlayerPauseGameUI;
    // Serialization 为外部序列化工具，所以友元从方便性来看是必要的
    friend class Serialization;
public:
    TileLevel(MainWindow *, int, int);
    ~TileLevel();

    // 格子地图的组织结构
protected:
    int mapRows = 0, mapColumns = 0, mapSize = 0;
    Actor** map = nullptr;
    int currentTileCount = 0;
public:
    inline int getMapSize() const { return mapSize; }
    inline int getMapRows() const { return mapRows; }
    inline int getMapColumns() const { return mapColumns; }

    // Helper functions for position transformation
public:
    // 格点左上角的世界坐标与格点坐标
    QPointF gridToWorld(QPoint) const;
    QPointF gridToWorld(int, int) const;
    QPoint worldToGrid(QPointF) const;
    // map 索引与格点坐标
    inline int gridToLinear(QPoint pos) const { return pos.x() * mapColumns + pos.y(); }
    inline int gridToLinear(int posx, int posy) const { return posx * mapColumns + posy; };
    inline int linearToGridRow(int pos) const { return pos / mapColumns; }
    inline int linearToGridColumn(int pos) const { return pos % mapColumns; }
    inline QPoint linearToGrid(int pos) const { return QPoint(pos / mapColumns, pos % mapColumns); }
    // 世界坐标与其属于的格子
    QPoint worldAtGrid(QPointF pos) const;
    // 格点之间的曼哈顿距离
    int gridManhattenDistance(QPoint, QPoint) const;
    // 获取整个格子地图、特定格点的 Rect
    QRectF gridRect(QPoint) const;
    QRectF mapRect() const;
    // 判断越界
    bool gridOutbound(QPoint) const;
    bool worldPointOutbound(QPointF) const;
    bool worldRectOutbound(QRectF) const;
    // 获取所有 tile，由于返回指针所以 const 只是为了在语法上兼容 const 的对象
    QList<Tile*> getTiles() const;
    // 获取所有 prop
    QList<Prop*> getProps() const;

protected:
    // 随机选取一个空白的格点
    QPoint chooseAvailableGrid() const;
    // 打乱所有 tile
    void mapShuffle();
    // 获取所有经角色移动可触及的 tile
    QList<QPoint> getReachableTiles(Character*) const;

    // 检测块的消除和执行块的消除
protected:
    // 经过两个途径点/一个途径转折点
    bool testTileEliminationTwoPass(QPoint, QPoint, QPoint&, QPoint&) const;
    bool testTileEliminationSinglePass(QPoint, QPoint, QPoint&) const;
    virtual void tileEliminationResponse(Character*) = 0;
public:
    bool tryTileElimination(Tile*, Tile*, Character*);

    // 检测显然的无解并结束游戏
protected:
    bool testNoSolution(Character*);
    virtual void noSolutionResponse(Character*) = 0;
public:
    void checkNoSolution(Character*);

    // 地图流程控制
protected:
    void levelInitialization() override;
protected:
    // 生成格子地图并显示
    void generateTileMap();
    // 根据已经生成的格子地图生成随机的角色起始点
    QRectF generateRandomStartPoint() const;
    // 放置角色，根据游戏模式的不同，放置角色数量也不同
    virtual void placeMatcher() = 0;    
protected:
    // 游戏结束、暂停
    bool gamePaused = false;
    void pauseActors(bool);
    virtual void gamePause(bool);
protected slots:
    void toggleGamePause();
protected:
    virtual void gameEnd(GameEndCause cause);
protected:
    // 在基础更新（更新 controller 和 actors）之上的更新
    virtual void customAdvance() override;

    // 倒计时
protected:
    static const int totalDowncount = 90;
    QTimeLine downcountTimer;
    int downcount = totalDowncount;
public:
    inline int getDowncount() const { return downcount;};
protected:
    // 不同的角色数量对应不同的倒计时归零处理
    virtual void downcountZeroResponse() = 0;
protected slots:
    void regularDowncountDecrease();

    // 道具生成器
protected:
    PropGenerator *propGen;

    // debug
public:
    void printMap();
};

#endif // TILELEVEL_H
