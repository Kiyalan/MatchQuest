#include <QtTest>
#include "tile.h"
#include <random>

class NoSolutionTest : public QObject
{
    Q_OBJECT
public:
    NoSolutionTest()
    {

    };
    ~NoSolutionTest()
    {

    };
private:
    int mapRows = 6, mapColumns = 6, mapSize = 36, *map;
    // NoSolutionTest 使用到的 helper function
    inline bool gridOutbound(QPoint pos) const
    {
        return pos.x() < 0 || pos.x() >= mapRows || pos.y() < 0 || pos.y() >= mapColumns;
    }
    inline int gridToLinear(int x, int y) const
    {
        return x * mapColumns + y;
    }
    inline int gridToLinear(QPoint pos) const
    {
        return pos.x() * mapColumns + pos.y();
    }
    inline QList<QPoint> getReachableTiles(QPoint src)
    {
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
        // 利用 BFS 搜索出所有通过移动可以接触到的 tile
        QQueue<QPoint> que;
        QList<QPoint> result;
        if (gridOutbound(src))
            return QList<QPoint>();
        que.enqueue(src);
        visit[gridToLinear(src)] = 1;
        while (!que.empty()) {
            QPoint current = que.dequeue();
            for (int i = 0; i < 4; ++i) {
                QPoint target {current + mov[i]};
                if (available(target)) {
                    // 用 -1 表示此格子为空
                    bool blocked = map[gridToLinear(target)] != -1;
                    if (blocked) {
                        result.append(target);
                    } else {
                        que.enqueue(target);
                    }
                    visit[gridToLinear(target)] = 1;
                }
            }
        }
        return result;
    }
    // 检测两个 tile 是否可以通过两次以内的折线相连
    bool testTileEliminationTwoPass(QPoint a, QPoint b, QPoint &pass1, QPoint &pass2) const
    {
        if (gridOutbound(a) || gridOutbound(b))
            qFatal() << "Fatal: TileLevel::testTileElimination two pass input grid outbound";
        const auto testHorizontal = [&](QPoint p1, QPoint p2)
        {
            int row = p1.x(), col1 = p1.y(), col2 = p2.y();
            if (col1 > col2)
                std::swap(col1, col2);
            for (int col = col1 + 1; col < col2; ++col)
                if (map[gridToLinear(row, col)] != -1)
                    return false;
            return true;
        };
        const auto testVertical = [&](QPoint p1, QPoint p2)
        {
            int col = p1.y(), row1 = p1.x(), row2 = p2.x();
            if (row1 > row2)
                std::swap(row1, row2);
            for (int row = row1 + 1; row < row2; ++row)
                if (map[gridToLinear(row, col)] != -1)
                    return false;
            return true;
        };
        QList<QPoint> aStretch;
        for (int i = 0; i < mapRows; ++i) {
            QPoint target {i, a.y()};
            if (map[gridToLinear(target.x(), target.y())] == -1 && testVertical(target, a))
                aStretch.append(target);
        }
        for (int i = 0; i < mapColumns; ++i) {
            QPoint target {a.x(), i};
            if (map[gridToLinear(target.x(), target.y())] == -1 && testHorizontal(target, a))
                aStretch.append(target);
        }
        QList<QPoint> bStretch;
        for (int i = 0; i < mapRows; ++i) {
            QPoint target {i, b.y()};
            if (map[gridToLinear(target.x(), target.y())] == -1 && testVertical(target, b))
                bStretch.append(target);
        }
        for (int i = 0; i < mapColumns; ++i) {
            QPoint target {b.x(), i};
            if (map[gridToLinear(target.x(), target.y())] == -1 && testHorizontal(target, b))
                bStretch.append(target);
        }
        // qDebug() << "    Detail: a Stretch: " << aStretch;
        // qDebug() << "    Detail: b Stretch: " << bStretch;
        if ((a.x() == b.x() && testHorizontal(a, b)) || (a.y() == b.y() && testVertical(a, b))) {
            // qDebug() << "    Detail: Succeed direct conection";
            pass1 = pass2 = a;
            return true;
        } else for (auto &p1 : aStretch)
                for (auto &p2 : bStretch)
                    if ((p1.x() == p2.x() && testHorizontal(p1, p2)) || (p1.y() == p2.y() && testVertical(p1, p2))) {
                        // qDebug() << "    Detail: Succeed with " << p1 << " and " << p2;
                        // 连接的顺序为 a -> pass1 -> pass2 -> b
                        pass1 = p1, pass2 = p2;
                        return true;
                    }
        // qDebug() << "Process: tile connection between " << a << " and " << b << " : " << (leastDistance < inf);
        return false;
    }
    bool calculateNoSolution(QPoint src)
    {
        const int colorCount = static_cast<size_t>(Tile::TileType::TileTypeCount);
        QList<QPoint> colorBucket[colorCount];
        QList<QPoint> reach = getReachableTiles(src);
        if (reach.empty())
            return false;
        for (auto &x : reach) {
            int color = map[gridToLinear(x)];
            colorBucket[color].append(x);
        }
        QPoint placeHolder;
        for (int i = 0; i < colorCount; ++i) {
            for (int l = 0; l < colorBucket[i].size(); ++l)
                for (int r = l + 1; r < colorBucket[i].size(); ++r)
                    // 由于不需要判定路径信息，所以这里用 placeHolder 充当占位符
                    if (testTileEliminationTwoPass(colorBucket[i][l], colorBucket[i][r], placeHolder, placeHolder))
                        return false;
        }
        return true;
    };
private slots:
    void testNoSolution1() {
        mapRows = mapColumns = 3;
        QPoint src {0, 0};
        int init[] {
            -1, 5, 5,
            1, 1, 5,
            5, 1, 1
        };
        bool answer = true;
        mapSize = mapRows * mapColumns;
        map = new int[mapSize];
        for (int i = 0; i < mapSize; ++i)
            map[i] = init[i];
        QVERIFY(calculateNoSolution(src) == answer);
        delete map;
    }
    void testNoSolution2() {
        mapRows = mapColumns = 6;
        QPoint src {2, 0};
        int init[] {
            3, 3, 2, 2, -1, 4,
            4, 2, -1, 2, 4, -1,
            -1, 4, 0, 0, 0, 0,
            3, 3, 0, 0, 2, 2,
            2, 2, 4, 4, 2, 2,
            4, 4, 5, 5, 5, 5
        };
        bool answer = false;
        mapSize = mapRows * mapColumns;
        map = new int[mapSize];
        for (int i = 0; i < mapSize; ++i)
            map[i] = init[i];
        QVERIFY(calculateNoSolution(src) == answer);
        delete map;
    }
    void testNoSolution3() {
        mapRows = mapColumns = 3;
        QPoint src {0, 1};
        int init[] {
            0, -1, 0,
            2, 2, 3,
            3, 5, 5
        };
        bool answer = false;
        mapSize = mapRows * mapColumns;
        map = new int[mapSize];
        for (int i = 0; i < mapSize; ++i)
            map[i] = init[i];
        QVERIFY(calculateNoSolution(src) == answer);
        delete map;
    }
    void testNoSolution4() {
        mapRows = 7,  mapColumns = 5;
        QPoint src {3, 0};
        int init[] {
            1, 1, 3, 3, 3,
            3, 4, 4, 1, 1,
            -1, 5, 5, 3, 3,
            -1, 3, 3, 1, 1,
            2, 2, 3, 3, 2,
            -1, 2, 0, 0, 1,
            1, 1, 1, 2, 2
        };
        bool answer = false;
        mapSize = mapRows * mapColumns;
        map = new int[mapSize];
        for (int i = 0; i < mapSize; ++i)
            map[i] = init[i];
        QVERIFY(calculateNoSolution(src) == answer);
        delete map;
    }
    void testNoSolution5() {
        mapRows = 8, mapColumns = 7;
        QPoint src {0, 6};
        int init[] {
            2, 2, 1, 1, 4, 4, -1,
            -1, 2, 2, 5, 5, -1, 2,
            2, 5, 5, 4, 4, 5, -1,
            5, 0, 0, 3, 3, 5, 5,
            4, 4, -1, 5, 5, 2, 2,
            0, 0, 4, 4, 1, 1, 2,
            2, 4, 4, 5, 5, -1, 0,
            0, 3, 3, 4, 4, 2, 2
        };
        bool answer = true;
        mapSize = mapRows * mapColumns;
        map = new int[mapSize];
        for (int i = 0; i < mapSize; ++i)
            map[i] = init[i];
        QVERIFY(calculateNoSolution(src) == answer);
        delete map;
    }
    void testNoSolution6() {
        mapRows = 7, mapColumns = 3;
        QPoint src {1, 0};
        int init[] {
            5, 5, 0,
            -1, 0, -1,
            4, 4, 4,
            4, 3, 3,
            4, 4, 2,
            2, 0, 0,
            3, -1, 3
        };
        bool answer = true;
        mapSize = mapRows * mapColumns;
        map = new int[mapSize];
        for (int i = 0; i < mapSize; ++i)
            map[i] = init[i];
        QVERIFY(calculateNoSolution(src) == answer);
        delete map;
    }
    void testNoSolution7() {
        mapRows = mapColumns = 7;
        QPoint src {6, 1};
        int init[] {
            2, 2, 2, -1, 2, 4, 4,
            4, 4, 4, 4, -1, 3, -1,
            3, 1, 1, 3, 3, 0, 0,
            2, 2, 4, 4, 0, 0, 3,
            3, 2, -1, 2, 3, 3, 0,
            0, 4, 4, 2, 2, 0, 0,
            4, -1, 4, 1, 1, 4, 4
        };
        bool answer = false;
        mapSize = mapRows * mapColumns;
        map = new int[mapSize];
        for (int i = 0; i < mapSize; ++i)
            map[i] = init[i];
        QVERIFY(calculateNoSolution(src) == answer);
        delete map;
    }
    void testNoSolution8() {
        mapRows = 5,  mapColumns = 3;
        QPoint src {1, 1};
        int init[] {
            1, 1, 1,
            1, -1, 0,
            0, 5, 5,
            1, 1, 4,
            4, 0, 0,
        };
        bool answer = false;
        mapSize = mapRows * mapColumns;
        map = new int[mapSize];
        for (int i = 0; i < mapSize; ++i)
            map[i] = init[i];
        QVERIFY(calculateNoSolution(src) == answer);
        delete map;
    }
    void testNoSolution9() {
        mapRows = mapColumns = 6;
        QPoint src {2, 0};
        int init[] {
            3, 3, 2, 2, -1, 4,
            4, 2, -1, 2, 4, -1,
            -1, 4, 0, 0, 0, 0,
            3, 3, 0, 0, 2, 2,
            2, 2, 4, 4, 2, 2,
            4, 4, 5, 5, 5, 5
        };
        bool answer = false;
        mapSize = mapRows * mapColumns;
        map = new int[mapSize];
        for (int i = 0; i < mapSize; ++i)
            map[i] = init[i];
        QVERIFY(calculateNoSolution(src) == answer);
        delete map;
    }
    void testNoSolution10() {
        mapRows = 8, mapColumns = 4;
        QPoint src {0, 3};
        int init[] {
            1, 1, -1, 4,
            4, -1, 4, 4,
            2, 2, 2, 2,
            5, 5, 1, 1,
            1, 1, 3, 3,
            1, 1, 3, 3,
            1, 1, -1, 2,
            2, 3, 3, -1,
        };
        bool answer = false;
        mapSize = mapRows * mapColumns;
        map = new int[mapSize];
        for (int i = 0; i < mapSize; ++i)
            map[i] = init[i];
        QVERIFY(calculateNoSolution(src) == answer);
        delete map;
    }
};

QTEST_APPLESS_MAIN(NoSolutionTest)



#include "nosolutiontest.moc"
