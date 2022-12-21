#include <algorithm>
#include <cassert>
#include <optional>
#include <ranges>

#include <QCoreApplication>
#include <QDebug>
#include <QFile>
#include <QHash>
#include <QList>
#include <QMap>
#include <QPoint>
#include <QQueue>
#include <QRegularExpression>
#include <QSet>

struct Elem
{
	qsizetype priority;
	qint64 value;
};

constexpr qsizetype reduce(qsizetype size, qsizetype index)
{
	if (index >= size)
		return index % (size - 1);

	if (index < 0) {
		index = ((index - 0) % (size - 1)) + (size - 1);
	}

	return index;
}

void step(QList<Elem> &list, qsizetype index)
{
	qsizetype value = list[index].value;

	value = reduce(list.size(), value);
	if (value == 0)
		return;

	qsizetype dstIndex = reduce(list.size(), index + value);
	list.move(index, dstIndex);
}

void process(QList<Elem> &list)
{
	for (qsizetype priority = 0; priority < list.size(); ++priority) {
		const auto it = std::ranges::find_if(std::as_const(list), [priority](const Elem &e) { return e.priority == priority; });
		Q_ASSERT(it != list.constEnd());
		auto index = it - list.constBegin();
		step(list, index);
	}
}

void solve(const QString &filename)
{
	QFile file(filename);

	if (!file.open(QFile::ReadOnly)) {
		qFatal("Failed to open file");
	}

	QList<Elem> list;
	QList<Elem> listPart2;
	qsizetype priority = 0;
	while (!file.atEnd()) {
		const auto line = file.readLine().trimmed();
		if (line.isEmpty()) {
			continue;
		}
		bool ok = false;
		qint64 value = line.toLongLong(&ok);
		Q_ASSERT(ok);
		list.append(Elem{
		    .priority = priority,
		    .value = value,
		});

		listPart2.append(Elem{
		    .priority = priority,
		    .value = value * 811589153,
		});

		priority += 1;
	}

	// Part 1
	process(list);
	auto zero = std::ranges::find_if(std::as_const(list), [](const Elem &e) { return e.value == 0; }) - list.constBegin();
	qint64 val1 = list[(zero + 1000) % list.size()].value;
	qint64 val2 = list[(zero + 2000) % list.size()].value;
	qint64 val3 = list[(zero + 3000) % list.size()].value;
	qInfo() << "1:" << val1 + val2 + val3;

	// Part 2
	for (int i = 0; i < 10; ++i) {
		process(listPart2);
	}
	zero = std::ranges::find_if(std::as_const(listPart2), [](const Elem &e) { return e.value == 0; }) - listPart2.constBegin();

	val1 = listPart2[(zero + 1000) % listPart2.size()].value;
	val2 = listPart2[(zero + 2000) % listPart2.size()].value;
	val3 = listPart2[(zero + 3000) % listPart2.size()].value;

	qInfo() << "2:" << val1 + val2 + val3;
}

int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);

	solve("test.txt");
	solve("input.txt");

	return 0;
}
