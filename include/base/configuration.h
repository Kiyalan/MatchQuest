#pragma once
#ifndef CONFIGURATION_H
#define CONFIGURATION_H
#include <qtypes.h>
#include <QColor>

class Configuration
{
public:
    inline static const int milisecondsPerFrame = 1000 / 60;
    inline static const qreal secondsPerFrame = milisecondsPerFrame / 1000.;
    inline static const QColor sceneBackgroundColor {0xCCD7C5};
    inline static const QColor tileHighlightColor {0xFDE74C};
};

#endif // CONFIGURATION_H
