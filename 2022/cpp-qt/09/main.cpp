#include <algorithm>
#include <ranges>

#include <QCoreApplication>
#include <QFile>
#include <QDebug>
#include <QList>
#include <QPoint>
#include <QSet>

template<typename T>
constexpr T sign(T val)
{
    if (val > 0)
        return 1;
    else if (val == 0)
        return 0;
    else
        return -1;
}

class Knot {
public:
    QPoint pos;
    Knot *before = nullptr;
    QSet<QPoint> visited;
    void updateTail()
    {
        if (before) {
            auto delta = before->pos - pos;
            if (std::abs(delta.x()) < 2 && std::abs(delta.y()) < 2) {
                delta = {0, 0};
            } else {
                delta = { sign(delta.x()), sign(delta.y())};
            }
            pos += delta;
        }
        visited.insert(pos);
    }

};

class Game {
public:
    Game()
    {
        Knot *previous = nullptr;
        for (int i = 0 ; i < 10 ; ++i) {
            auto knot = new Knot();
            knot->before = previous;
            knots.append(knot);
            previous = knot;
        }
        tail = previous;
    }

    void up() {
        move(0, 1);
    }

    void down()
    {
        move(0, -1);
    }


    void right() {
        move(1, 0);
    }

    void left()
    {
        move(-1, 0);
    }

    void move(int dx, int dy)
    {
        knots.front()->pos += QPoint{dx, dy};
        for (auto knot : knots)
        {
            knot->updateTail();
        }
        qDebug() << tail->pos;
    }



    auto count() const
    {
        return tail->visited.size();
    }
private:
    QList<Knot *> knots;
    Knot *tail;
};

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QFile file("input.txt");

    if (!file.open(QFile::ReadOnly)) {
        return -1;
    }

    Game game;
    while (!file.atEnd()) {
        const auto line = file.readLine().trimmed();
        qDebug() << line;
        const auto split = line.split(' ');
        Q_ASSERT(split.size() == 2);

        bool ok = false;
        int count = split[1].toInt(&ok);
        Q_ASSERT(ok);

        using FUNC = void (Game::*)();
        FUNC func = nullptr;

        if (split[0] == "U") {
            func = &Game::up;
        } else if (split[0] == "D") {
            func = &Game::down;
        } else if (split[0] == "L") {
            func = &Game::left;
        } else if (split[0] == "R") {
            func = &Game::right;
        }

        Q_ASSERT(func != nullptr);

        for (int i = 0 ; i < count ; ++i)
            (game.*func)();
    }


    qInfo() << game.count();
    return 0;
}

