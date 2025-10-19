#include "matcher.h"
#include "configuration.h"
#include "tilelevel.h"
#include "tile.h"
#include "prop.h"
#include "configuration.h"
#include "matchercontroller.h"
#include <QPainter>
#include <QObject>

Matcher::Matcher(Actor* parent, QString avatarResourceAbsolutePath)
    : Character(parent)
{
    if (!avatar.load(avatarResourceAbsolutePath))
        qFatal() << "Fatal: Matcher::Matcher failed to load matcher image: " << avatarResourceAbsolutePath;
    // 将图片拉伸至设定大小
    avatar = avatar.scaled(matcherSize, matcherSize);
    // 进行圆形裁剪
    QRectF circleRect (0, 0, matcherSize, matcherSize);
    // QRectF circleMarginFixRect (0, 0, matcherSize + 1, matcherSize + 1);
    QPixmap clipAvatar (matcherSize + 1, matcherSize + 1);
    clipAvatar.fill(Qt::transparent);
    QPainter clipAvatarPainter(&clipAvatar);
    // 在 clipAvatar 上画圆形背景
    clipAvatarPainter.setBrush(QColor(0x5BC0EB));
    clipAvatarPainter.setPen(Qt::NoPen);
    clipAvatarPainter.drawEllipse(circleRect);
    clipAvatarPainter.setRenderHint(QPainter::Antialiasing);
    // 设置裁剪区域
    QPainterPath clipPath;
    clipPath.addEllipse(circleRect);
    clipAvatarPainter.setClipPath(clipPath);
    // 绘制裁剪的 avatar
    clipAvatarPainter.drawPixmap(circleRect, avatar, circleRect);
    // clipAvatarPainter.setBrush(Qt::NoBrush);
    // clipAvatarPainter.setPen(QPen(Qt::black, 3));
    // clipAvatarPainter.drawEllipse(circleRect);
    avatar = clipAvatar;
}

QRectF Matcher::boundingRect() const
{
    return QRectF(-matcherSize / 2., -matcherSize / 2., matcherSize, matcherSize);
}

QPainterPath Matcher::shape() const
{
    QPainterPath path;
    path.addEllipse(QRectF(-matcherSize / 2., -matcherSize / 2., matcherSize, matcherSize));
    return path;
}

void Matcher::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    // 处理移动残留百边的初次尝试
    // painter->setRenderHint(QPainter::Antialiasing, false);
    // painter->setRenderHint(QPainter::SmoothPixmapTransform, false);
    painter->translate(-matcherSize / 2., -matcherSize / 2.);
    painter->drawPixmap(QRect(0, 0, matcherSize, matcherSize), avatar, QRect(0, 0, matcherSize, matcherSize));
}

// 处理移动残留白边的初次尝试，被 FullViewportUpdate 的解决方案替代
// QVariant Matcher::itemChange(GraphicsItemChange change, const QVariant &value)
// {
//     if (change == ItemPositionChange && scene()) {
//         prepareGeometryChange();
//         QPointF newPos = value.toPointF();
//         QPointF alignedNewPos(std::round(newPos.x()), std::round(newPos.y()));

//         QRectF oldRect = sceneBoundingRect();
//         QRectF newRect = mapRectToScene(boundingRect().translated(alignedNewPos - pos()));

//         QRectF combinedRect = oldRect.united(newRect);
//         QRectF expandedRect = combinedRect.adjusted(-50, 50, 50, 50);
//         QRectF AlignedRect = QRectF(round(expandedRect.x()), round(expandedRect.y()), round(expandedRect.width()), round(expandedRect.height()));
//         scene()->update(AlignedRect);

//         return alignedNewPos;
//     }
//     return QGraphicsObject::itemChange(change, value);
// }

void Matcher::advance(int phase)
{
    if (phase == 0) return;
    // qDebug() << "Process: Matcher " << this << " advance begins";
    QPointF oldPos = pos();
    QPointF newPos = oldPos + velocity * Configuration::secondsPerFrame;
    setPos(newPos);
    auto tileScene = static_cast<TileLevel*>(scene());
    if (!tileScene)
        qFatal() << "Fatal: Matcher::advance, collide with Tile, tile Scene empty";
    const QList<QGraphicsItem*> collisions = collidingItems();
    if (!collisions.isEmpty()) {
        for (auto x : collisions) {
            // qDebug() << "Process: Matcher " << this << " check collision with Tile";
            void* ptr = dynamic_cast<Tile*>(x);
            // 如果与 Tile 发生碰撞
            if (ptr) {
                Tile *tile = static_cast<Tile*>(ptr);
                qDebug() << "Process: Matcher " << this << " touch tile " << tile->pos();
                if (!lastTouchTile) {
                    tile->setTouched(true);
                    lastTouchTile = tile;
                } else if (tile == lastTouchTile) {

                } else {
                    if (tileScene->tryTileElimination(tile, lastTouchTile, this)) {
                        lastTouchTile = nullptr;
                        tileScene->checkNoSolution(this);
                    } else {
                        lastTouchTile->setTouched(false);
                        tile->setTouched(true);
                        lastTouchTile = tile;
                    }
                }
                x = nullptr;
                setPos(oldPos);
            }
            // qDebug() << "Process: Matcher " << this << " check collision with Prop";
            ptr = dynamic_cast<Prop*>(x);
            if (ptr) {
                Prop *prop = static_cast<Prop*>(ptr);
                qDebug() << "Process: Matcher " << this << " touch prop " << prop->pos();
                auto matcherControl = static_cast<MatcherController*>(controller);
                if (!matcherControl)
                    qFatal() << "Fatal: Matcher::advance, collide with prop, matcher controller empty";
                matcherControl->applyPropEffect(prop->getPropType(), this);
                tileScene->removeItem(prop);
                delete prop;
                x = nullptr;
                setPos(oldPos);
            }

            ptr = dynamic_cast<Matcher*>(x);
            if (ptr) {
                x = nullptr;
            }
        }
    } else {
        QRectF myRect {0, 0, matcherSize, matcherSize};
        myRect.moveCenter(newPos);
        if (tileScene->worldRectOutbound(myRect))
            setPos(oldPos);
    }
    // qDebug() << "     Detail: after collision detect:" << tileScene->items();
    // qDebug() << "Process: Matcher " << this << " advance ends";
}

void Matcher::moveUp(int dir)
{
    velocity.ry() -= dir * matcherMovingSpeed;
}
void Matcher::moveRight(int dir)
{
    velocity.rx() += dir * matcherMovingSpeed;
}
void Matcher::stopMoving()
{
    velocity.rx() = velocity.ry() = 0.;
}
