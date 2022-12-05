#include <QCoreApplication>
#include <QFile>
#include <QDebug>
#include <QList>
#include <QStack>

enum class State {
    InitialState,
    Movements
};

class Stacks {
public:
    void init (const QList<QByteArray> &input)
    {
        auto line = input.last();
        stacks.resize(qsizetype(line.size() / 4));

        for (int i = input.size() - 2 ; i >= 0 ; --i) {
            const auto line = input.at(i);
            for (int stackIndex = 0 ; stackIndex < stacks.size() ; ++stackIndex) {
                char c = line[stackIndex * 4 + 1];
                if (c == ' ')
                    continue;
                stacks[stackIndex].push(c);
            }
        }
    }

    void move(int from, int to) {
        char c = stacks[from-1].pop();
        stacks[to-1].push(c);
    }

    void move(int from, int to, int qty) {
        QStack<char> buffer;

        for (int i = 0 ; i < qty ; ++i) {
            buffer.push(stacks[from -1].pop());

        }
        for (int i = 0 ; i < qty ; ++i) {
            stacks[to -1].push(buffer.pop());
        }
    }

    QList<QStack<char>> stacks;
};

int main(int argc, char *argv[])
{
    State state = State::InitialState;
    QCoreApplication a(argc, argv);

    QFile file("input.txt");

    if (!file.open(QFile::ReadOnly)) {
        return -1;
    }

    QList<QByteArray> initial;
    Stacks stacks;
    while (!file.atEnd()) {
        const auto line = file.readLine();

        if (state == State::InitialState) {
            if (line == "\n") {
                stacks.init(initial);
                state = State::Movements;
                continue;
            }

            initial.append(line);
        } else {
            auto split = line.split(' ');
            qDebug() << split;
            int qty = split[1].toInt();
            int from = split[3].toInt();
            int to = split[5].toInt();

            stacks.move(from, to, qty);
        }

    }

    QByteArray result;
    for (const auto &stack: stacks.stacks) {
        result += stack.top();
    }

    qDebug() << result;

    return 0;
}
