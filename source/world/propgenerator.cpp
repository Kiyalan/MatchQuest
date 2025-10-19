#include "propgenerator.h"

#include "prop.h"
#include "tilelevel.h"
#include <QTimer>
#include <random>

PropGenerator::PropGenerator(QList<Prop::PropType> propTypeList, int genDelta, Actor* parent)
    : EmptyGraphicsItem(parent), genTimeLine(genDelta, this), generationDeltaTime(genDelta)
{
    genListLength = propTypeList.length();
    genList = new Prop::PropType[genListLength];
    for (int i = 0; i < genListLength; ++i)
        genList[i] = propTypeList[i];
    // 启动 Prop 生成的定时器
    genTimeLine.setLoopCount(0);
    connect(&genTimeLine, &QTimeLine::valueChanged, this, [this](qreal value)
    {
        static qreal lastValue = 0;
        if (value < lastValue)
            this->generate();
        lastValue = value;
    });
    genTimeLine.start();
}
PropGenerator::~PropGenerator()
{
    genTimeLine.stop();
    delete genList;
}

void PropGenerator::generate()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    if (genListLength == 0)
        return;
    std::uniform_int_distribution<int> distType(0, genListLength - 1);
    // 生成随机的类型
    Prop::PropType genType = static_cast<Prop::PropType>(distType(gen));
    // 生成随机的位置，通过调用 tileLevel 的 chooseAvailableGrid 实现
    TileLevel *tileLevel = static_cast<TileLevel*>(scene());
    if (!tileLevel)
        qFatal() << "Fatal: PropGenerator::generate(): tileLevel empty";
    QPointF propPos {tileLevel->gridRect(tileLevel->chooseAvailableGrid()).center()};
    // 创建 prop
    auto prop = new Prop(genType, propPos, tileLevel->getRoot());
    qDebug() << "Process: Prop generator generate prop of " << Prop::propImagePathTable[static_cast<int>(prop->getPropType())] << " at " << prop->pos();
}

void PropGenerator::pause(bool p)
{
    qDebug() << "Process: prop generator pause " << p;
    if (p && !paused) {
        genTimeLine.stop();
        qDebug() << "    Detail: current Time" << genTimeLine.currentTime();
    }else if (!p && paused) {
        genTimeLine.resume();
        qDebug() << "    Detail: current Time" << genTimeLine.currentTime();
    }
    Actor::pause(p);
}
