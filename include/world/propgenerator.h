#pragma once
#ifndef PROPGENERATOR_H
#define PROPGENERATOR_H

#include "emptygraphicsitem.h"
#include "prop.h"
#include <QTimeLine>

class PropGenerator : public EmptyGraphicsItem
{
    Q_OBJECT
public:
    static const int defaultGenerationDeltaTime = 10000;
public:
    PropGenerator(QList<Prop::PropType> propTypeList, int genDelta = defaultGenerationDeltaTime, Actor* parent = nullptr);
    ~PropGenerator();
protected:
    QTimeLine genTimeLine;
    Prop::PropType *genList;
    int genListLength;
    int generationDeltaTime;
protected:
    virtual void pause(bool) override;
public slots:
    void generate();
};


#endif // PROPGENERATOR_H
