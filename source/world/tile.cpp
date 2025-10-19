#include "tile.h"
#include "tilelevel.h"
#include "configuration.h"
#include <QPainterStateGuard>
#include <QPen>

Tile::Tile(Actor *parent, QPointF pos, TileType itype)
    : Actor(parent), type(itype), appearance(tileAppranceTable[static_cast<int>(itype)])
{
    setPos(pos);
}

Tile::~Tile()
{

}

void Tile::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    // 不需要手动保存
    // QPainterStateGuard guard(painter);
    painter->setBrush(appearance);
    QColor frameColor = touched ? Configuration::tileHighlightColor : Qt::black;
    painter->setPen(QPen(frameColor, 2.5));
    painter->drawRect(0, 0, tileSize, tileSize);
}

QRectF Tile::boundingRect() const
{
    return QRectF(0, 0, tileSize, tileSize);
}

void Tile::setTouched(bool in)
{
    // 触发重绘
    if (touched != in)
        update();
    // 如果处于高亮状态将 Z 值提高为 1, 这样高亮框不会被遮住
    if (touched)
        setZValue(0);
    else
        setZValue(1);
    touched = in;
}
