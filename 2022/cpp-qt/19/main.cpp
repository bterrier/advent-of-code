#include <algorithm>
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

enum class Resource
{
	Ore,
	Clay,
	Obsidian,
	Geode
};

static QDebug operator<<(QDebug debug, Resource r)
{

	switch (r) {

	case Resource::Ore:
		debug << "Ore";
		break;
	case Resource::Clay:
		debug << "Clay";
		break;
	case Resource::Obsidian:
		debug << "Obsidian";
		break;
	case Resource::Geode:
		debug << "Geode";
		break;
	}

	return debug;
}

using Recipe = QMap<Resource, int>;
using Costs = QMap<Resource, Recipe>;

struct Blueprint
{
	int id;
	Costs costs;
};

using Stocks = QHash<Resource, int>;

struct State
{
	QHash<Resource, int> robots;
	Stocks stocks;
	QSet<Resource> blacklist;
};

struct Game
{
	explicit Game(const Costs &costs)
	    : costs(costs)
	{
		states.append(State{
		    {
		        {Resource::Ore, 1},
		    },
		    {},
		});

		for (const auto &recipe : costs) {
			for (auto it = recipe.constBegin(); it != recipe.constEnd(); ++it) {
				maxes[it.key()] = qMax(maxes[it.key()], it.value());
			}
		}
	}
	QList<State> states;
	const Costs costs;
	QMap<Resource, int> maxes;
	int geod = 0;
	int round = 0;

	int hope(const State &state) const
	{
		return state.stocks[Resource::Geode] + (32 - round) / 2 * (2 * state.robots[Resource::Geode] + (32 - round) - 1);
	}
	void prune()
	{
		int max = 0;
		for (const auto &state : std::as_const(states)) {
			max = qMax(state.stocks[Resource::Geode], max);
		}
		geod = max;
		auto [first, last] = std::ranges::remove_if(states, [max, this](State state) -> bool { return hope(state) < max; });
		states.erase(first, last);
	}

	bool canBuild(Resource robotType, const Stocks &stocks)
	{
		Recipe recipe = costs[robotType];

		for (auto it = recipe.constBegin(); it != recipe.constEnd(); ++it) {
			if (stocks[it.key()] < it.value()) {
				return false;
			}
		}

		return true;
	}

	void step()
	{
		QList<State> newStates;
		for (const auto &state : std::as_const(states)) {
			auto usableStocks = state.stocks;
			auto newStocks = state.stocks;
			for (auto it = state.robots.constBegin(); it != state.robots.constEnd(); ++it) {
				if (it.value() > 0) {
					newStocks[it.key()] += it.value();
				}
			}

			auto newRobots = state.robots;
			if (canBuild(Resource::Geode, usableStocks)) {
				Recipe recipe = costs[Resource::Geode];
				for (auto it = recipe.constBegin(); it != recipe.constEnd(); ++it) {
					newStocks[it.key()] -= it.value();
				}
				newRobots[Resource::Geode] += 1;
				newStates.append(State{newRobots, newStocks});
				continue;
			}

			State newState{newRobots, newStocks};
			for (Resource r2 : {Resource::Clay, Resource::Obsidian, Resource::Ore}) {

				if (canBuild(r2, usableStocks)) {
					newState.blacklist.insert(r2);
				}
			}
			newStates.append(newState);

			for (Resource r : {Resource::Clay, Resource::Obsidian, Resource::Ore}) {
				if (!canBuild(r, usableStocks))
					continue;
				if (newRobots[r] >= maxes[r])
					continue;
				if (state.blacklist.contains(r))
					continue;

				State newState{newRobots, newStocks};

				Recipe recipe = costs[r];
				for (auto it = recipe.constBegin(); it != recipe.constEnd(); ++it) {
					newState.stocks[it.key()] -= it.value();
				}

				newState.robots[r] += 1;
				newStates.append(newState);
			}
		}

		states = newStates;
		++round;
		prune();
	}
};

void solve(const QString &filename)
{
	QFile file(filename);

	if (!file.open(QFile::ReadOnly)) {
		qFatal("Failed to open file");
	}

	static QRegularExpression regex(R"(^Blueprint (?<id>\d+): Each ore robot costs (?<ore_ore>\d+) ore\. Each clay robot costs (?<clay_ore>\d+) ore. Each obsidian robot costs (?<obsidian_ore>\d+) ore and (?<obsidian_clay>\d+) clay. Each geode robot costs (?<geode_ore>\d+) ore and (?<geode_obsidian>\d+) obsidian.$)");

	qint64 result = 0;
	qint64 result2 = 1;
	while (!file.atEnd()) {
		const auto line = file.readLine().trimmed();
		if (line.isEmpty()) {
			continue;
		}

		const auto match = regex.match(line);
		Blueprint bp;
		bool ok = false;
		bp.id = match.captured("id").toInt(&ok);
		Q_ASSERT(ok);
		bp.costs[Resource::Ore][Resource::Ore] = match.captured("ore_ore").toInt(&ok);
		Q_ASSERT(ok);
		bp.costs[Resource::Clay][Resource::Ore] = match.captured("clay_ore").toInt(&ok);
		Q_ASSERT(ok);
		bp.costs[Resource::Obsidian][Resource::Ore] = match.captured("obsidian_ore").toInt(&ok);
		Q_ASSERT(ok);
		bp.costs[Resource::Obsidian][Resource::Clay] = match.captured("obsidian_clay").toInt(&ok);
		Q_ASSERT(ok);
		bp.costs[Resource::Geode][Resource::Ore] = match.captured("geode_ore").toInt(&ok);
		Q_ASSERT(ok);
		bp.costs[Resource::Geode][Resource::Obsidian] = match.captured("geode_obsidian").toInt(&ok);
		Q_ASSERT(ok);

		Game game(bp.costs);

		for (int i = 0; i < 24; ++i) {
			game.step();
		}

		int max = 0;
		for (const auto &state : std::as_const(game.states)) {
			max = qMax(max, state.stocks[Resource::Geode]);
		}

		result += game.geod * bp.id;

		if (bp.id <= 3) {
			for (int i = 24; i < 32; ++i) {
				game.step();
			}
			result2 *= game.geod;
		}
	}
	qInfo() << "1:" << result;
	qInfo() << "2:" << result2;
}

int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);

	solve("test.txt");
	solve("input.txt");

	return 0;
}
