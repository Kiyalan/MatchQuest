#include "tilelevel.h"
#include "tile.h"
#include "Character.h"
#include "tileelimination.h"
#include <utility>
#include <algorithm>
#include <random>
#include <QQueue>
#include <QTimer>
#include "emptygraphicsitem.h"
#include "matchercontroller.h"
#include "propgenerator.h"

TileLevel::TileLevel(MainWindow *window, int row, int col)
    : Level(window), mapRows(row), mapColumns(col), mapSize(row * col), downcountTimer(1000, this)
{
    // 初始化地图
    map = new Actor* [mapSize];
    for (int i = 0; i < mapSize; ++i)
        map[i] = nullptr;
    generateTileMap();
    // 维护倒计时
    downcountTimer.setLoopCount(0);
    connect(&downcountTimer, &QTimeLine::valueChanged, this, [this](qreal value)
    {
        static qreal lastValue = 0;
        if (value < lastValue)
            this->regularDowncountDecrease();
        lastValue = value;
    });
    downcountTimer.start();
}

TileLevel::~TileLevel()
{
    delete[] map;
}

// 实现网格坐标到世界坐标的转换，小数据类型不使用 const QPoint&
// 注意 grid 坐标是 (row, col) 对应是 (y, x) 而不是 (x, y)
QPointF TileLevel::gridToWorld(QPoint grid) const
{
    return QPointF(Tile::tileSize * grid.y(), Tile::tileSize * grid.x());
}

QPointF TileLevel::gridToWorld(int x, int y) const
{
    return QPointF(Tile::tileSize * y, Tile::tileSize * x);
}

QPoint TileLevel::worldToGrid(QPointF pos) const
{
    return QPoint(qRound(pos.y() / Tile::tileSize), qRound(pos.x() / Tile::tileSize));
}

QPoint TileLevel::worldAtGrid(QPointF pos) const
{
    return QPoint(qFloor(pos.y() / Tile::tileSize), qFloor(pos.x() / Tile::tileSize));
}

int TileLevel::gridManhattenDistance(QPoint a, QPoint b) const
{
    return std::abs(a.x() - b.x()) + std::abs(a.y() - b.y());
}

QRectF TileLevel::gridRect(QPoint grid) const
{
    return QRectF(gridToWorld(grid), QSizeF(Tile::tileSize, Tile::tileSize));
}

QRectF TileLevel::mapRect() const
{
    return QRectF(0, 0, mapColumns * Tile::tileSize, mapRows * Tile::tileSize);
}

bool TileLevel::gridOutbound(QPoint pos) const
{
    return pos.x() < 0 || pos.x() >= mapRows || pos.y() < 0 || pos.y() >= mapColumns;
}

bool TileLevel::worldPointOutbound(QPointF pos) const
{
    return pos.x() < 0 || pos.x() > mapRect().right() || pos.y() < 0 || pos.y() > mapRect().bottom();
}

bool TileLevel::worldRectOutbound(QRectF rect) const
{
    return rect.left() < mapRect().left() || rect.right() > mapRect().right() || rect.bottom() > mapRect().bottom() || rect.top() < mapRect().top();
}

QList<Tile*> TileLevel::getTiles() const
{
    QList<Tile*> result;
    for (int i = 0; i < mapSize; ++i)
        if (map[i] != nullptr)
            result.append(static_cast<Tile*>(map[i]));
    return result;
}

QList<Prop*> TileLevel::getProps() const
{
    QList<Prop*> result;
    for (auto x : items()) {
        Prop* y = dynamic_cast<Prop*>(x);
        if (y)
            result.append(y);
    }
    return result;
}

QPoint TileLevel::chooseAvailableGrid() const
{
    std::random_device rd;
    std::mt19937 gen(rd());
    if (currentTileCount >= mapSize)
        qFatal() << "Fatal: TileLevel::chooseAvailableGrid no available grid";
    std::uniform_int_distribution<int> dist(0, mapSize - currentTileCount - 1);
    int pos = dist(gen);
    for (int i = 0, j = 0; ; ++i)
        if (map[i] == nullptr) {
            if (j == pos) {
                qDebug() << "Process: tile level chosen available grid " << linearToGrid(i);
                return linearToGrid(i);
            }
            else ++j;
        }
}

void TileLevel::mapShuffle()
{
    qDebug() << "Process: tile level conduct map shuffling";
    std::random_device rd;
    std::mt19937 gen(rd());
    // Fisher-Yates 算法在所有 Tiles 之间重排
    int indices[currentTileCount];
    for (int i = 0, j = 0; i < mapSize; ++i)
        if (map[i] != nullptr)
            indices[j++] = i;
    for (int i = currentTileCount - 1; i > 0; --i) {
        std::uniform_int_distribution<int> dist(0, i - 1);
        int j = dist(gen);
        auto tilei = static_cast<Tile*>(map[indices[i]]), tilej = static_cast<Tile*>(map[indices[j]]);
        qDebug() << "    Detail: swap " << linearToGrid(indices[i]) << " with " << linearToGrid(indices[j]);
        shuffleSwap(*tilei, *tilej);
    }
    qDebug() << "Process: tile level map shuffling completed";
}

QList<QPoint> TileLevel::getReachableTiles(Character* player) const
{
    if (!player)
        qFatal() << "Fatal: TileLevel::getReachableTiles player is nullptr";
    qDebug() << "Process: tile level acquire reachable tiles for " << player;
    static const QPoint mov[4] = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};
    bool *visit = new bool[mapSize];
    for (int i = 0; i < mapSize; ++i)
        visit[i] = 0;
    const auto available = [&](QPoint p)
    {
        if (gridOutbound(p))
            return false;
        bool visited = visit[gridToLinear(p)];
        return !visited;
    };
    QQueue<QPoint> que;
    QList<QPoint> result;
    QPoint src = worldAtGrid(player->scenePos());
    if (gridOutbound(src))
        qFatal() << "Fatal: TileLevel::getReachableTiles source point outbound";
    que.enqueue(src);
    visit[gridToLinear(src)] = 1;
    while (!que.empty()) {
        QPoint current = que.dequeue();
        for (int i = 0; i < 4; ++i) {
            QPoint target {current + mov[i]};
            if (available(target)) {
                Tile *tile = static_cast<Tile*>(map[gridToLinear(target)]);
                bool blocked = (tile != nullptr);
                if (blocked) {
                    result.append(target);
                } else {
                    que.enqueue(target);
                }
                visit[gridToLinear(target)] = 1;
            }
        }
    }
    qDebug() << "Process: tile level found reachable tiles:" << result;
    return result;
}


void TileLevel::gameEnd(GameEndCause cause)
{
    qDebug() << "Process: tile level game end in with" << gameEndCauseToString(cause);
    gamePaused = true;
    pauseActors(true);
    downcountTimer.stop();
}


void TileLevel::pauseActors(bool p)
{
    QList<QGraphicsItem *> actorList = items();
    for (auto &x : actorList) {
        auto actor = static_cast<Actor*>(x);
        if (!actor)
            qFatal() << "Fatal: TileLevel::pauseActors item not actor pause = " << p;
        actor->pause(p);
    }
}
void TileLevel::gamePause(bool p)
{
    qDebug() << "Process: tile level game pause " << p;
    auto player = static_cast<MatcherController*>(controller);
    if (!player)
        qFatal() << "Fatal: TileLevel::gamePause player empty or not matcher controller";
    if (!gamePaused && p) {
        gamePaused = true;
        pauseActors(true);
        player->gamePause(true);
        downcountTimer.stop();
    } else if (gamePaused && !p) {
        gamePaused = false;
        pauseActors(false);
        player->gamePause(false);
        downcountTimer.resume();
    }
}

void TileLevel::toggleGamePause(){
    gamePause(!gamePaused);
}

void TileLevel::customAdvance()
{
    auto control = static_cast<MatcherController*>(controller);
    if (!control)
        qFatal() << "Fatal: TileLevel::advance controller empty or not matcher controller";
    if (gamePaused)
        return;
    // qDebug() << "Process: tile level custom advance";
    control->advance();
    Level::advance();
}

void TileLevel::generateTileMap()
{
    qDebug() << "Process: tile level generate initial map";
    static const qreal fillRatio = 0.9;
    int genTileCount = qRound(mapSize * fillRatio) / 2 * 2;
    int *tmp = new int[mapSize];
    int *result = new int[mapSize];
    for (int i = 0; i < mapSize; ++i)
        tmp[i] = i;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::sample(tmp, tmp + mapSize, result, genTileCount, gen);
    int colorCount = static_cast<int>(Tile::TileType::TileTypeCount);
    std::uniform_int_distribution<int> colorDist(0, colorCount - 1);
    for (int i = 0; i < genTileCount; i += 2) {
        int color = colorDist(gen);
        QPoint grid1 {linearToGrid(result[i])};
        map[result[i]]     = new Tile(root, gridToWorld(grid1), static_cast<Tile::TileType>(color));
        QPoint grid2 {linearToGrid(result[i + 1])};
        map[result[i + 1]] = new Tile(root, gridToWorld(grid2), static_cast<Tile::TileType>(color));
    }
    currentTileCount = genTileCount;
    delete[] tmp;
    delete[] result;
    printMap();
    qDebug() << "Process: tile level generated initial map";
}

QRectF TileLevel::generateRandomStartPoint() const
{
    return gridRect(chooseAvailableGrid());
}

void TileLevel::levelInitialization()
{
    // 设置初始相机锚点为地图中心偏右 1/4
    QRectF mapArea = mapRect();
    cameraPivot->setPos(controller->getViewSize().width() * 1. / 6 + mapArea.width() * 1. / 2, mapArea.center().y());
    // qDebug() << "    Detail: " << cameraPivot;
}

// 检测两个点是否可以通过三段段以内的折现连接
bool TileLevel::testTileEliminationTwoPass(QPoint a, QPoint b, QPoint &pass1, QPoint &pass2) const
{
    if (gridOutbound(a) || gridOutbound(b))
        qFatal() << "Fatal: TileLevel::testTileElimination two pass input grid outbound";
    qDebug() << "Process: tile level test tile connection two pass between " << a << " and " << b;
    const auto testHorizontal = [&](QPoint p1, QPoint p2)
    {
        int row = p1.x(), col1 = p1.y(), col2 = p2.y();
        if (col1 > col2)
            std::swap(col1, col2);
        for (int col = col1 + 1; col < col2; ++col)
            if (map[gridToLinear(row, col)] != nullptr)
                return false;
        return true;
    };
    const auto testVertical = [&](QPoint p1, QPoint p2)
    {
        int col = p1.y(), row1 = p1.x(), row2 = p2.x();
        if (row1 > row2)
            std::swap(row1, row2);
        for (int row = row1 + 1; row < row2; ++row)
            if (map[gridToLinear(row, col)] != nullptr)
                return false;
        return true;
    };
    QList<QPoint> aStretch;
    for (int i = 0; i < mapRows; ++i) {
        QPoint target {i, a.y()};
        if (map[gridToLinear(target.x(), target.y())] == nullptr && testVertical(target, a))
            aStretch.append(target);
    }
    for (int i = 0; i < mapColumns; ++i) {
        QPoint target {a.x(), i};
        if (map[gridToLinear(target.x(), target.y())] == nullptr && testHorizontal(target, a))
            aStretch.append(target);
    }
    QList<QPoint> bStretch;
    for (int i = 0; i < mapRows; ++i) {
        QPoint target {i, b.y()};
        if (map[gridToLinear(target.x(), target.y())] == nullptr && testVertical(target, b))
            bStretch.append(target);
    }
    for (int i = 0; i < mapColumns; ++i) {
        QPoint target {b.x(), i};
        if (map[gridToLinear(target.x(), target.y())] == nullptr && testHorizontal(target, b))
            bStretch.append(target);
    }
    // qDebug() << "    Detail: a Stretch: " << aStretch;
    // qDebug() << "    Detail: b Stretch: " << bStretch;
    static const int inf = 1e9;
    int leastDistance = inf;
    if ((a.x() == b.x() && testHorizontal(a, b)) || (a.y() == b.y() && testVertical(a, b))) {
        // qDebug() << "    Detail: Succeed direct conection";
        pass1 = pass2 = a;
        leastDistance = gridManhattenDistance(a, b);
    } else for (auto &p1 : aStretch)
        for (auto &p2 : bStretch)
            if ((p1.x() == p2.x() && testHorizontal(p1, p2)) || (p1.y() == p2.y() && testVertical(p1, p2))) {
                // qDebug() << "    Detail: Succeed with " << p1 << " and " << p2;
                // 连接的顺序为 a -> pass1 -> pass2 -> b
                int dist = gridManhattenDistance(a, p1) + gridManhattenDistance(p1, p2) + gridManhattenDistance(p2, b);
                if (leastDistance == inf || dist < leastDistance)
                    pass1 = p1, pass2 = p2,
                    leastDistance = dist;
            }
    qDebug() << "Process: tile connection between " << a << " and " << b << " : " << (leastDistance < inf);
    if (leastDistance < inf)
        return true;
    else
        return false;
}

// 检测两个点是否可以通过两段以内的折现连接
bool TileLevel::testTileEliminationSinglePass(QPoint a, QPoint b, QPoint &pass) const
{
    if (gridOutbound(a) || gridOutbound(b))
        qFatal() << "Fatal: TileLevel::testTileElimination single pass input grid outbound";
    qDebug() << "Process: tile level test tile connection single pass between " << a << " and " << b;
    const auto testHorizontal = [&](QPoint p1, QPoint p2)
    {
        int row = p1.x(), col1 = p1.y(), col2 = p2.y();
        if (col1 > col2)
            std::swap(col1, col2);
        for (int col = col1 + 1; col < col2; ++col)
            if (map[gridToLinear(row, col)] != nullptr)
                return false;
        return true;
    };
    const auto testVertical = [&](QPoint p1, QPoint p2)
    {
        int col = p1.y(), row1 = p1.x(), row2 = p2.x();
        if (row1 > row2)
            std::swap(row1, row2);
        for (int row = row1 + 1; row < row2; ++row)
            if (map[gridToLinear(row, col)] != nullptr)
                return false;
        return true;
    };
    QList<QPoint> aStretch;
    for (int i = 0; i < mapRows; ++i) {
        QPoint target {i, a.y()};
        if (map[gridToLinear(target.x(), target.y())] == nullptr && testVertical(target, a))
            aStretch.append(target);
    }
    for (int i = 0; i < mapColumns; ++i) {
        QPoint target {a.x(), i};
        if (map[gridToLinear(target.x(), target.y())] == nullptr && testHorizontal(target, a))
            aStretch.append(target);
    }
    // qDebug() << "    Detail: a Stretch: " << aStretch;
    static const int inf = 1e9;
    int leastDistance = inf;
    if ((a.x() == b.x() && testHorizontal(a, b)) || (a.y() == b.y() && testVertical(a, b))) {
        // qDebug() << "    Detail: Succeed direct conection";
        pass = a;
        leastDistance = gridManhattenDistance(a, b);
    } else for (auto &p : aStretch)
        if ((p.x() == b.x() && testHorizontal(p, b)) || (p.y() == b.y() && testVertical(p, b))) {
            // qDebug() << "    Detail: Succeed with " << p1;
            // 连接的顺序为 a -> pass -> b
            int dist = gridManhattenDistance(a, p) + gridManhattenDistance(p, b);
            if (leastDistance == inf || dist < leastDistance)
                pass = p,
                leastDistance = dist;
        }
    qDebug() << "Process: tile connection between " << a << " and " << b << " : " << (leastDistance < inf);
    if (leastDistance < inf)
        return true;
    else
        return false;
}


bool TileLevel::tryTileElimination(Tile* a, Tile* b, Character* player)
{
    if (!a || !b)
        qFatal() << "Fatal: tileLevel::tryTileElimination input tile null";
    if (!player)
        qFatal() << "Fatal: tileLevel::tryTileElimination input tile null";
    qDebug() << "Process: tile level try tile elimination between " << a << " and " << b;
    if (a->getTileType() != b->getTileType())
        return false;
    QPoint pos[4] {worldToGrid(a->pos()), QPoint(0, 0), QPoint(0, 0), worldToGrid(b->pos())};
    if (testTileEliminationTwoPass(pos[0], pos[3], pos[1], pos[2])) {
        // 移除成功匹配的箱子
        // qDebug() << "    Detail: before deleting tiles" << items().size() << items();
        map[gridToLinear(pos[0])] = map[gridToLinear(pos[3])] = nullptr;
        removeItem(a), removeItem(b);
        // delete a, delete b;
        // qDebug() << "    Detail: after deleting tiles" << items().size() << items();
        currentTileCount -= 2;
        // 生成匹配成功的连线粒子效果
        new TileElimination(pos, 1000, TileElimination::defaultPen, root);
        // 触发箱子消除的关卡更新，如分数、无解检测
        tileEliminationResponse(player);
        // qDebug() << "    Detail: after calling tileEliminationResponse" << items().size() << items();
        return true;
    } else
        return false;
    qDebug() << "Process: tile level try tile elimination between " << a << " and " << b;
}

// 仅仅基于当前状况进行一个无解的充分不必要的判断：可以接触到的箱子没有可以被消除的的。箱子的颜色数量配对已经由生成过程保证
bool TileLevel::testNoSolution(Character* player)
{
    if (player == nullptr)
        qFatal() << "Fatal: TileLevel::testNoSolution player is nullptr";
    qDebug() << "Process: tile level test no solution for " <<  player;
    const int colorCount = static_cast<size_t>(Tile::TileType::TileTypeCount);
    QList<QPoint> colorBucket[colorCount];
    QList<QPoint> reach = getReachableTiles(player);
    if (reach.empty())
        return false;
    for (auto &x : reach) {
        int color = static_cast<int>(static_cast<Tile*>(map[gridToLinear(x)])->getTileType());
        colorBucket[color].append(x);
    }
    QPoint placeHolder;
    for (int i = 0; i < colorCount; ++i) {
        for (int l = 0; l < colorBucket[i].size(); ++l)
            for (int r = l + 1; r < colorBucket[i].size(); ++r)
                // 由于不需要判定的路径信息，所以这里用 placeHolder 充当占位符
                if (testTileEliminationTwoPass(colorBucket[i][l], colorBucket[i][r], placeHolder, placeHolder))
                    return false;
    }
    return true;
}

void TileLevel::checkNoSolution(Character* player)
{
    if (player == nullptr)
        qFatal() << "Fatal: TileLevel::checkNoSolution player is nullptr";
    qDebug() << "Process: tile level check solution for player " << player;
    if (testNoSolution(player)) {
        qDebug() << "Process: tile level verified no solution";
        // 触发无解的关卡变化，这会结束游戏
        noSolutionResponse(player);
    } else {
        qDebug() << "Process: tile level verified solution existence";
    }
    // qDebug() << "    Detail: after testing" << items().size() << items();
}



void TileLevel::printMap()
{
    // 为 NoSolutionTest 生成数据的 helper function
    auto comma = QString(", ");
    for (int i = 0; i < mapRows; ++i) {
        QString s;
        for (int j = 0; j < mapColumns; ++j) {
            Tile *tile = static_cast<Tile*>(map[gridToLinear(i, j)]);
            if (tile != nullptr)
                s += QString::number(static_cast<int>(tile->getTileType()));
            else
                s += "-1";
            s += comma;
        }
        qDebug().noquote() << s;
    }
}


void TileLevel::regularDowncountDecrease()
{
    // 触发时间减少导致的关卡变化
    if (--downcount == 0)
        downcountZeroResponse();
}
