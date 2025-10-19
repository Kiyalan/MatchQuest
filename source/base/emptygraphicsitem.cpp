#include "emptygraphicsitem.h"

EmptyGraphicsItem::EmptyGraphicsItem(Actor* parent)
    : Actor(parent)
{

}
QRectF EmptyGraphicsItem::boundingRect() const
{
    return QRectF(0, 0, 0, 0);
}
void EmptyGraphicsItem::paint(QPainter *, const QStyleOptionGraphicsItem *, QWidget *)
{

}

