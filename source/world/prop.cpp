#include "prop.h"
#include <QGraphicsObject>
#include <QPainter>

Prop::Prop(PropType inType, QPointF pos, Actor* parent)
    : Actor(parent), type(inType)
{
    setPos(pos);
    QString path = propImagePathTable[static_cast<size_t>(inType)];
    if (!image.load(path))
        qFatal() << "Fatal: Prop::Prop failed to open prop image:" << path;
    image = image.scaled(propSize, propSize);
}

Prop::~Prop()
{

}

void Prop::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    painter->translate(-propSize / 2., -propSize / 2.);
    painter->drawPixmap(QRect(0, 0, propSize, propSize), image, QRect(0, 0, propSize, propSize));
}

QRectF Prop::boundingRect() const
{
    return QRectF(-propSize / 2., -propSize / 2., propSize, propSize);
}
