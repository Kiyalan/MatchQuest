
#pragma once
#ifndef PROP_H
#define PROP_H

#include "actor.h"

class Character;

// 不放在类内是因为在不在 CMakeLists 中改 C++ 标准的版本时（>= C++17），无法向前声明嵌套在类中的枚举类
enum class PropType
{
    TimeProlong,
    Shuffle,
    Hint,
    Flash,
    Freeze,
    Dizzy,
    PropTypeCount = 6
};

class Prop : public Actor
{
    Q_OBJECT
    // Serialization 为外部序列化工具，所以友元从方便性来看是必要的
    friend class Serialization;
public:
    using PropType = ::PropType;
    inline static const QString propImagePathTable[static_cast<size_t>(PropType::PropTypeCount)]
    {
        ":/image/clock.png",
        ":/image/shuffle.png",
        ":/image/hints.png",
        ":/image/flash.png",
        ":/image/snowflake.png",
        ":/image/dizzy.png"
    };
    static constexpr qreal propSize = 60.;
public:
    Prop(PropType, QPointF, Actor*);
    ~Prop();
protected:
    PropType type;
    QPixmap image;
public:
    inline PropType getPropType() const { return type; }
protected:
    void paint(QPainter *, const QStyleOptionGraphicsItem *, QWidget *widget = nullptr) override;
    QRectF boundingRect() const override;

};

#endif // PROP_H
