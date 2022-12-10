#include <algorithm>
#include <ranges>

#include <QCoreApplication>
#include <QFile>
#include <QDebug>
#include <QList>
#include <QPoint>
#include <QSet>
#include <QQueue>


class Cpu
{
public:
    Cpu(){
        queue.enqueue(0);
    }
    void incycle()
    {
        if (((cycle - 20) % 40) == 0) {
            sig += cycle * reg;
        }

        qDebug() << cycle << reg << sig;
        auto crt_index = (cycle - 1)%40;
        crt.append((crt_index == reg || crt_index == reg - 1 || crt_index == reg + 1) ? '#' : '.');
    }
    void postcycle()
    {
        const auto val = queue.dequeue();
        qDebug() << cycle << val;
        reg += val;
    }

    void run(const QByteArray &instruction)
    {
        cycle += 1;

        const auto split = instruction.split(' ');

        const auto opcode = split[0];
        int latency = 0;
        if (opcode == "noop") {
            queue.enqueue(0);
        } else if (opcode == "addx") {
            queue.enqueue(split[1].toLongLong());
            latency = 1;
        }

        incycle();
        postcycle();

        while (latency--)
        {
            run("noop");
        }
    }
    qint64 cycle = 0;
    qint64 reg = 1;
    QQueue<qint64> queue;
    qint64 sig = 0;
    QByteArray crt;

};

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QFile file("input.txt");

    if (!file.open(QFile::ReadOnly)) {
        return -1;
    }

    Cpu cpu;
    while (!file.atEnd()) {
        const auto line = file.readLine().trimmed();
        cpu.run(line);
    }


    // Part 1: Signal strength
    qInfo() << cpu.sig;

    // Draw CRT
    qInfo() << cpu.crt.mid(0, 40);
    qInfo() << cpu.crt.mid(40, 40);
    qInfo() << cpu.crt.mid(80, 40);
    qInfo() << cpu.crt.mid(120, 40);
    qInfo() << cpu.crt.mid(160, 40);
    qInfo() << cpu.crt.mid(200, 40);
    return 0;
}

