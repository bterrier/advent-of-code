#include <algorithm>
#include <ranges>

#include <QCoreApplication>
#include <QFile>
#include <QDebug>
#include <QList>
#include <QSet>

QList<int> findtrees(const QList<int> range) {
    int currentLevel = -1;
    QList<int> positions;

    for (int i = 0 ; i < range.size() ; ++i) {
        if (range[i] > currentLevel) {
            positions.append(i);
            currentLevel = range[i];
        }
    }

    currentLevel = -1;

    for (int i = range.size() - 1 ; i >= 0 ; --i) {
        if (range[i] > currentLevel) {
            positions.append(i);
            currentLevel = range[i];
        }
    }
    return positions;
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QFile file("input.txt");

    if (!file.open(QFile::ReadOnly)) {
        return -1;
    }

    using Tree = QPair<int, int>;
    QSet<Tree> visibleTrees;
    QList<QList<int>> columns;
    QList<QList<int>> rows;
    int currentRow = 0;
    while (!file.atEnd()) {
        const auto line = file.readLine().trimmed();

        if (columns.isEmpty()) {
            columns.resize(line.size());
        }
        QList<int> row;
        row.resize(line.length());
        std::ranges::transform(line, row.begin(), [](char c)->int{ return c - '0'; });

        for (int column = 0 ; column < columns.size() ; ++column) {
            columns[column].append(row.at(column));
        }

        const auto list = findtrees(row);
        for (int pos : list)
            visibleTrees.insert({currentRow, pos});

        currentRow += 1;
        rows.append(row);
    }
    for (int column = 0 ; column < columns.size() ; ++column) {
        const auto list = findtrees(columns[column]);

        for (int pos : list)
            visibleTrees.insert({pos, column});
    }

    qInfo() << visibleTrees.count();
    int best = 0;

    for (int i = 1 ; i < rows.size() -1 ; ++i) {

        for (int j = 1 ; j < columns.size() - 1; ++j) {

            int up = 0;
            int down = 0;
            int left = 0;
            int right = 0;

            int index;
            for (index = i + 1 ; index < columns[j].size() -1 ; ++ index) {
                if (columns[j][index] >= columns[j][i]) {
                    break;
                }
            }
            down = index - i;


            for (index = j + 1 ; index < rows[i].size()-1 ; ++ index) {
                if (rows[i][index] >= rows[i][j]) {
                    break;
                }
            }
            right = index - j;

            for (index = i - 1 ; index > 0 ; --index) {
                if (columns[j][index] >= columns[j][i]) {
                    break;
                }
            }
            up = i- index;


            for (index = j - 1 ; index > 0 ; -- index) {
                if (rows[i][index] >= rows[i][j]) {
                    break;
                }
            }
            left = j- index;

            int score = up * left*down * right;
            if (score > best) {
                best = score;
            }
        }

    }

    qInfo() << best;
    return 0;
}

