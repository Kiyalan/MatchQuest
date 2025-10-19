#ifndef DUALPLAYERLEVEL_H
#define DUALPLAYERLEVEL_H

#include "tilelevel.h"

class DualPlayerLevel : public TileLevel
{
    Q_OBJECT
    friend class DualMatcherController;
    // Serialization 为外部序列化工具，所以友元从方便性来看是必要的
    friend class Serialization;
public:
    DualPlayerLevel(MainWindow *, int, int);
protected:
    void placeMatcher() override;
    void levelInitialization() override;
protected:
    void tileEliminationResponse(Character*) override;
    void downcountZeroResponse() override;
    void noSolutionResponse(Character*) override;
    void gameEnd(GameEndCause) override;
protected:
    int score[2] = {0, 0};
public:
    inline int getScore(int i) { return score[i]; }
};

#endif // DUALPLAYERLEVEL_H
