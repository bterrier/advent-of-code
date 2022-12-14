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

class Item
{
public:

	enum Type {
		Number,
		List,
	};

	explicit Item(QByteArrayView str)
	{
		if (str.startsWith('[') && str.endsWith(']')) {
			m_type = List;
			if (str.length() > 2) {
				QList<int> commas;
				int depth = 0;
				QByteArrayView inner(str.constData() + 1, str.length() - 2);
				for (int i = 0 ; i < inner.length() ; ++i) {
					if (inner[i] == '[') {
						++depth;
					} else if (inner[i] == ']') {
						--depth;
					} else if (depth == 0 && inner[i] == ',') {
						commas.append(i);
					}
				}
				commas.append(inner.length());
				int last = -1;
				for (int c : commas) {
					int from = last + 1;
					int to = c;
					const auto slice = inner.sliced(from, to - from);
					m_list.append(Item{slice});
					last = c;
				}

			}
		} else {
			m_type = Number;
			bool ok = false;
			m_number = str.toInt(&ok);
			Q_ASSERT(ok);
		}
	}

	static Item wrap(Item i) {
		Item item;
		item.m_list.append(i);
		return item;
	}

private:
	// Shouldn't be need but MSVC won't use <=> otherwise...
	friend bool operator == (const Item &a, const Item &b) {
		return a <=> b == 0;
	}
	friend std::strong_ordering operator <=> (const Item &a, const Item &b)
	{
		if (a.m_type == Number && b.m_type == Number){
			return a.m_number <=> b.m_number;
		} else if (a.m_type == List && b.m_type == List){
			const auto length = std::min(a.m_list.size(), b.m_list.size());

			for (int i = 0 ; i < length ; ++i) {
				auto r = a.m_list.at(i) <=> b.m_list.at(i);
				if (r == std::strong_ordering::equal)
					continue;

				return r;
			}

			return a.m_list.size() <=> b.m_list.size();
		} else if (a.m_type == Number) {
			return Item::wrap(a) <=> b;
		} else {
			return a <=> Item::wrap(b);
		}
	}

	Item() :
	    m_type(List)
	{}

	Type m_type;
	qint64 m_number;
	QList<Item> m_list;
	QString toString() const {
		if (m_type == Number)
		{
			return QString::number(m_number);
		} else {
			QString str = "[";
			for (const auto &item : m_list) {
				str.append(item.toString());
				str.append(',');
			}
			str.chop(1);
			str.append("]");

			return str;
		}
	}
};


int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);

	QFile file("input.txt");

	if (!file.open(QFile::ReadOnly)) {
		return -1;
	}

	QList<Item> items;
	QList<Item> dividers {
		Item{"[[2]]"},
		Item{"[[6]]"},
	};
	QList<Item> packets = dividers;
	int currentPair = 1;
	int result = 0;
	while (!file.atEnd()) {
		const auto line = file.readLine().trimmed();
		if (line.isEmpty()) {
			packets.append(items);
			items.clear();
			currentPair += 1;
		} else {
			items.append(Item{line});
			if (items.size() == 2) {
				if (items[0] <= items[1]) {
					result += currentPair;
				}
			}

			Q_ASSERT(items.size() <= 2);
		}
	}
	packets.append(items);

	qInfo() << result;

	// Part 2
	std::ranges::sort(packets);
	result = 1;
	for (int i = 0 ; i < packets.size() ; ++i) {
		if (dividers.contains(packets.at(i))) {
			result *= (i + 1);
		}
	}
	qInfo() << result;
	return 0;
}





