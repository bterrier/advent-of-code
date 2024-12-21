#include <QDebug>
#include <QFile>
#include <QString>

class PrintOrder
{
public:
    PrintOrder() = default;
    void addPage(int page) { m_pages.append(page); }
    int middlePage() const
    {
        if (m_pages.isEmpty())
            return 0;

        const int middle = m_pages.size() / 2;
        return m_pages[middle];
    }

    bool check(const QMultiHash<int, int> &rules) const
    {
        for (int i = 0; i < m_pages.size(); ++i) {
            for (int j = i + 1; j < m_pages.size(); ++j) {
                if (rules.contains(m_pages[j], m_pages[i])) {
                    return false;
                }
            }
        }

        return true;
    }

    void repair(const QMultiHash<int, int> &rules)
    {
        for (int i = 0; i < m_pages.size(); ++i) {
            for (int j = i + 1; j < m_pages.size(); ++j) {
                if (rules.contains(m_pages[j], m_pages[i])) {
                    m_pages.move(i, j);
                    i--;
                    break;
                }
            }
        }
    }

private:
    QList<int> m_pages;
};

void solve(const QString &filename)
{
    QFile file(filename);
    bool ok = file.open(QFile::ReadOnly | QFile::Text);
    Q_ASSERT(ok);

    QMultiHash<int, int> h;
    while (!file.atEnd()) {
        const auto line = file.readLine().trimmed();

        if (line.isEmpty()) {
            break;
        }

        const auto list = line.split('|');
        bool ok = false;
        Q_ASSERT(list.size() == 2);

        const auto a = list[0].toInt(&ok);
        Q_ASSERT(ok);

        const auto b = list[1].toInt(&ok);
        Q_ASSERT(ok);

        h.insert(a, b);
    }

    qint64 sum = 0;
    qint64 sum2 = 0;
    while (!file.atEnd()) {
        const auto line = file.readLine().trimmed();

        if (line.isEmpty()) {
            break;
        }

        const auto list = line.split(',');
        PrintOrder printOrder;

        for (const auto &str : list) {
            bool ok = false;

            printOrder.addPage(str.toInt(&ok));
            Q_ASSERT(ok);
        }

        if (printOrder.check(h)) {
            sum += printOrder.middlePage();
        } else {
            printOrder.repair(h);
            sum2 += printOrder.middlePage();
        }
    }

    qDebug() << sum;
    qDebug() << sum2;

    file.close();
}

int main(int, char *[])
{
    solve("test.txt");
    solve("input.txt");

    return 0;
}
