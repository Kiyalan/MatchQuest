#pragma once
#ifndef STARTMENULEVEL_H
#define STARTMENULEVEL_H

#include "level.h"

class StartMenuLevel : public Level
{
    Q_OBJECT
public:
    StartMenuLevel(MainWindow* window);
protected:
    void levelInitialization() override;
protected:
    void customAdvance() override;
};
#endif // STARTMENULEVEL_H
