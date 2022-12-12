#include <algorithm>
#include <ranges>
#include <optional>

#include <QCoreApplication>
#include <QFile>
#include <QDebug>
#include <QList>
#include <QPoint>
#include <QSet>
#include <QQueue>

struct Point {
	qsizetype x;
	qsizetype y;
};

struct Zone {
	enum Status {
		Unexplored,
		Accessible,
		Visited,
		End
	};

	int level = 255;
	int distance = 0;
	Status status = Unexplored;
};

class Map {
public:
	using Value = quint8;
	std::optional<Zone> at(Point point) const {
		if (point.x < 0 || point.y < 0 || point.x >= m_width || point.y >= m_height)
			return {};

		return m_data.at(index(point));
	}

	void update(Point point, Zone zone)
	{
		m_data[index(point)] = zone;
	}

	void addRow(QList<Zone> row) {
		if (m_data.isEmpty()) {
			m_data = row;
			m_width = row.size();
			m_height = 1;
		} else {
			Q_ASSERT(row.size() == m_width);
			m_height += 1;
			m_data.append(row);
		}
	}

	qsizetype width() const
	{
		return m_width;
	}

	qsizetype height() const
	{
		return m_height;
	}

	void resetStates() {
		for (auto &zone: m_data) {
			zone.distance = 0;
			zone.status = Zone::Unexplored;
		}
	}

private:
	qsizetype m_width = 0;
	qsizetype m_height = 0;
	[[nodiscard]] constexpr qsizetype index(Point point) const noexcept {
		return point.x + point.y * m_width;
	}
	QList<Zone> m_data;
};

int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);

	QFile file("input.txt");

	if (!file.open(QFile::ReadOnly)) {
		return -1;
	}

	Map map;
	Point start;
	Point end;
	QQueue<Point> queue;
	while (!file.atEnd()) {
		const auto line = file.readLine().trimmed();
		QList<Zone> row;
		for (char c : line) {
			Zone zone;
			if (c == 'S') {
				c = 'a';
				start = { row.size(), map.height()};
				zone.status = Zone::Accessible;
				zone.distance = 0;
				queue.enqueue(start);
			} else if (c == 'E') {
				c = 'z';
				zone.status = Zone::End;
				end = { row.size(), map.height()};
			}
			zone.level = c - 'a';
			row.append(zone);
		}
		map.addRow(row);
	}
	int result = -1;
	while (!queue.isEmpty()) {
		const auto point = queue.dequeue();
		auto current = map.at(point).value();
		const QList<Point> points = {
		    {point.x - 1, point.y},
		    {point.x + 1, point.y},
		    {point.x, point.y - 1},
		    {point.x, point.y + 1},
		};

		for (const auto &p : points) {
			auto maybeCandidate = map.at(p);
			if (!maybeCandidate.has_value()) // Out of bound
				continue;

			auto candidate = maybeCandidate.value();
			if (candidate.level > current.level + 1) { // Cannot climb
				continue;
			}

			if (candidate.status == Zone::End) {
				queue.clear();
				result = current.distance + 1;
				break;
			}

			if (candidate.status != Zone::Unexplored) { // Already went there
				continue;
			}

			queue.enqueue(p);
			candidate.distance = current.distance + 1;
			candidate.status = Zone::Accessible;
			map.update(p, candidate);
		}
		current.status = Zone::Visited;
		map.update(point, current);
	}
	qInfo() << result;

	result = -1;
	map.resetStates();
	queue.clear();
	queue.enqueue(end);
	while (!queue.isEmpty()) {
		const auto point = queue.dequeue();
		auto current = map.at(point).value();
		const QList<Point> points = {
		    {point.x - 1, point.y},
		    {point.x + 1, point.y},
		    {point.x, point.y - 1},
		    {point.x, point.y + 1},
		};

		for (const auto &p : points) {
			auto maybeCandidate = map.at(p);
			if (!maybeCandidate.has_value()) // Out of bound
				continue;

			auto candidate = maybeCandidate.value();
			if (candidate.level < current.level - 1) { // Cannot climb
				continue;
			}

			if (candidate.level == 0) {
				queue.clear();
				result = current.distance + 1;
				break;
			}

			if (candidate.status != Zone::Unexplored) { // Already went there
				continue;
			}

			queue.enqueue(p);
			candidate.distance = current.distance + 1;
			candidate.status = Zone::Accessible;
			map.update(p, candidate);
		}
		current.status = Zone::Visited;
		map.update(point, current);
	}

	qInfo() << result;

return 0;
}





