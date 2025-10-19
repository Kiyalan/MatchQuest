#pragma once
#include <qswap.h>
#ifndef TILE_H
#define TILE_H

#include "actor.h"

class Tile : public Actor
{
    Q_OBJECT
    // Serialization 为外部序列化工具，所以友元从方便性来看是必要的
    friend class Serialization;
public:
    enum class TileType
    {
        Malachite,
        SelectiveYellow,
        MexicanPink,
        BleuDeFrance,
        Chatreuse,
        BitterSweet,
        TileTypeCount = 6
    };
    static constexpr int tileAppranceTable[static_cast<size_t>(TileType::TileTypeCount)] =
    {
        0x04e762,
        0xf5b700,
        0xdc0073,
        0x008bf8,
        0x89fc00,
        0xEE6055
    };
    static constexpr qreal tileSize = 80.;
public:
    Tile(Actor*, QPointF, TileType);
    ~Tile();
private:
    TileType type;
    QColor appearance;
public:
    TileType getTileType() const { return type; }
protected:
    void paint(QPainter *, const QStyleOptionGraphicsItem *, QWidget *widget = nullptr) override;
    QRectF boundingRect() const override;
private:
    friend class Matcher;
    bool touched = false;
    void setTouched(bool);
public:
    friend void shuffleSwap(Tile& x, Tile& y) noexcept
    {
        auto tmpType = x.type;
        x.type = y.type;
        y.type = tmpType;
        auto tmpAppearance = x.appearance;
        x.appearance = y.appearance;
        y.appearance = tmpAppearance;
        x.touched = y.touched = 0;
        x.update(), y.update();
    }
};




#endif // TILE_H
