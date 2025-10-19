#include "serialization.h"
#include "matcher.h"
#include "singlematchercontroller.h"
#include "singleplayerlevel.h"
#include "dualmatchercontroller.h"
#include "dualplayerlevel.h"
#include "tile.h"
#include "prop.h"
#include "emptygraphicsitem.h"


Serialization* Serialization::getInstance()
{
    static Serialization* ins = new Serialization;
    return ins;
}

void Serialization::SerializeSinglePlayer(const SingleMatcherController* control, const SinglePlayerLevel* level, QDataStream& outStream) const
{
    qDebug() << "Process: serialization single player";
    outStream << int(1);
    outStream << level->getMapRows() << level->getMapColumns();
        qDebug() << level->getMapRows() << level->getMapColumns();
    const QList<Tile*> tiles = level->getTiles();
    outStream << int(tiles.size());
        qDebug() << "tiles size" << tiles.size();
    for (const Tile *x : tiles) {
        outStream << x->getTileType() << level->worldToGrid(x->pos());
            qDebug() << int(x->getTileType()) << level->worldToGrid(x->pos());
    }
    const QList<Prop*> props = level->getProps();
    outStream << int(props.size());
    for (const Prop *x : props)
        outStream << x->getPropType() << x->pos();
    outStream << level->getScore() << level->getDowncount();
        qDebug() << level->getScore() << level->getDowncount();
    outStream << control->getPlayer()->pos();
        qDebug() << control->getPlayer()->pos();
    qDebug() << "Process: serialization single player finished";
}
void Serialization::DeserializeSinglePlayer(SingleMatcherController* control, SinglePlayerLevel* level, QDataStream& inStream) const
{
    qDebug() << "Process: deserialization single player";
    inStream >> level->mapRows >> level->mapColumns;
        qDebug() << level->mapRows << level->mapColumns;
    level->mapSize = level->mapRows * level->mapColumns;
    auto items = level->items();
    for (auto x : items) {
        Tile *tile = dynamic_cast<Tile*>(x);
        if (tile) level->removeItem(tile);
        Prop *prop = dynamic_cast<Prop*>(x);
        if (prop) level->removeItem(prop);
    }
    for (int i = 0; i < level->mapSize; ++i)
        level->map[i] = nullptr;
    inStream >> level->currentTileCount;
        qDebug() << "tiles size" << level->currentTileCount;
    for (int i = 0; i < level->currentTileCount; ++i) {
        Tile::TileType type;
        QPoint pos;
        inStream >> type >> pos;
            qDebug() << int(type) << pos;
        level->map[level->gridToLinear(pos)] = new Tile(level->root, level->gridToWorld(pos), type);
    }
    int propsCount;
    inStream >> propsCount;
        qDebug() << propsCount;
    for (int i = 0; i < propsCount; ++i) {
        Prop::PropType type;
        QPointF pos;
        inStream >> type >> pos;
            qDebug() << int(type) << pos;
        new Prop(type, pos, level->root);
    }
    inStream >> level->score >> level->downcount;
        qDebug() << level->score << level->downcount;
    QPointF playerPos;
    inStream >> playerPos;
        qDebug() << playerPos;
    static_cast<Matcher*>(control->player)->setPos(playerPos);
    qDebug() << "Process: deserialization single player finished";
}
void Serialization::SerializeDualPlayer(const DualMatcherController* control, const DualPlayerLevel* level, QDataStream& outStream) const
{

}
void Serialization::DeserializeDualPlayer(DualMatcherController* control, DualPlayerLevel* level, QDataStream& inStream) const
{

}

