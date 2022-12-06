#include <QCoreApplication>
#include <QFile>
#include <QDebug>
#include <QList>
#include <QStack>

//PART ONE
// constexpr int MARKER_LENGTH = 4;
// PART TWO
constexpr int MARKER_LENGTH = 14;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QFile file("input.txt");

    if (!file.open(QFile::ReadOnly)) {
        return -1;
    }

    while (!file.atEnd()) {
        const auto line = file.readLine();

        for (int i = MARKER_LENGTH - 1 ; i < line.size() ; ++i) {
            QSet<char> set;
            for (int j = 0 ; j < MARKER_LENGTH; ++j) {
                set.insert(line[i - j]);
            }

            if (set.size() == MARKER_LENGTH) {
                qDebug() << i + 1;
                break;
            }
        }

    }

    return 0;
}
