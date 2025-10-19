#include "tileelimination.h"
#include "tilelevel.h"
#include "configuration.h"
#include <QPainter>
#include <QRect>

const int TileElimination::defaultDuration = 1000;
const QPen TileElimination::defaultPen = QPen(Configuration::tileHighlightColor, 10, Qt::PenStyle::SolidLine);

TileElimination::TileElimination(QPoint pos[4], int duration = defaultDuration, QPen connectPen = defaultPen, Actor* parent = nullptr)
    : Particle(parent, duration), drawPen(connectPen)
{
    qDebug() << "process: TileElimination created from " << pos[0] << " to " << pos[3];
    for (int i = 0; i < 4; ++i)
        node[i] = static_cast<TileLevel*>(scene())->gridRect(pos[i]).center();
    setZValue(-1);
    QRectF rect1 = QRectF(node[0], QSizeF{0., 0.});
    QRectF rect2 = QRectF(node[1], QSizeF{0., 0.});
    QRectF rect3 = QRectF(node[3], QSizeF{0., 0.});
    bounding = rect1.united(rect2).united(rect3);
}

TileElimination::~TileElimination()
{

}

void TileElimination::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    painter->setPen(drawPen);
    painter->setBrush(Qt::NoBrush);
    painter->drawLine(node[0], node[1]);
    painter->drawLine(node[1], node[2]);
    painter->drawLine(node[2], node[3]);
}

QRectF TileElimination::boundingRect() const
{
    return bounding;
}
