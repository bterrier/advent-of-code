#include <QDebug>
#include <QFile>
#include <QHash>
#include <QStack>
#include <QString>

using Int = qint64;
[[nodiscard]]
constexpr Int prune(Int a) noexcept
{
    return a % 16777216;
}

[[nodiscard]]
constexpr Int mix(Int a, Int b) noexcept
{
    return a ^ b;
}

[[nodiscard]]
constexpr Int next(Int a) noexcept
{
    a = prune(mix(a, a * 64));
    a = prune(mix(a, a / 32));
    return prune(mix(a, a * 2048));
}
[[nodiscard]]
constexpr Int next(Int a, Int steps) noexcept
{
    for (Int i = 0; i < steps; ++i) {
        a = next(a);
    }
    return a;
}
[[nodiscard]]
constexpr Int price(Int secretNumber) noexcept
{
    return secretNumber % 10;
}

class Sequence
{
public:
    template<typename... T>
    explicit Sequence(T &&...args)
        : m_changes{std::forward<T>(args)...}
    {}

private:
    friend QDebug operator<<(QDebug debug, const Sequence &s)
    {
        debug << s.m_changes[0] << s.m_changes[1] << s.m_changes[2] << s.m_changes[3];
        return debug;
    }

    friend bool operator==(const Sequence &a, const Sequence &b)
    {
        return a.m_changes == b.m_changes;
    }
    friend int qHash(const Sequence &s, size_t seed = 0)
    {
        return qHashRange(s.m_changes.begin(), s.m_changes.end(), seed);
    }
    const std::array<Int, 4> m_changes;
};

[[nodiscard]]
constexpr auto process(Int a, Int steps) noexcept
{
    QHash<Sequence, Int> hash;
    QStack<Int> stack;
    stack.reserve(steps);
    for (Int i = 0; i < steps; ++i) {
        const auto b = next(a);
        const auto delta = price(b) - price(a);
        stack.push(delta);
        a = b;

        if (i >= 3) {
            Sequence seq{
                stack.at(stack.size() - 4),
                stack.at(stack.size() - 3),
                stack.at(stack.size() - 2),
                stack.at(stack.size() - 1),
            };

            if (hash.contains(seq)) {
                continue;
            }
            //qDebug() << seq << price(a);
            hash.insert(seq, price(a));
        }
    }
    return hash;
}

Int solve(const QString &filename)
{
    qint64 result = 0;
    QFile file(filename);
    bool ok = file.open(QFile::ReadOnly | QFile::Text);
    Q_ASSERT(ok);

    QHash<Sequence, Int> globalHash;
    while (!file.atEnd()) {
        const auto line = file.readLine().trimmed();
        if (line.isEmpty())
            break;

        bool ok = false;
        const Int seed = line.toLongLong(&ok);
        Q_ASSERT(ok);
        result += next(seed, 2000);
        const auto hash = process(seed, 2000);
        for (auto it = hash.begin(); it != hash.end(); ++it) {
            auto git = globalHash.find(it.key());
            if (git == globalHash.end()) {
                globalHash.insert(it.key(), it.value());
            } else {
                git.value() += it.value();
            }
        }
    }
    auto values = globalHash.values();
    std::ranges::sort(values);
    qDebug() << values.last();
    file.close();

    return result;
}

int main(int, char *[])
{
    Q_ASSERT(mix(42, 15) == 37);
    Q_ASSERT(prune(100000000) == 16113920);
    Q_ASSERT(next(123) == 15887950);
    Q_ASSERT(next(15887950) == 16495136);
    Q_ASSERT(next(123, 1) == 15887950);
    Q_ASSERT(next(123, 2) == 16495136);
    qDebug() << "Test:" << solve("test.txt");
    qDebug() << "Result:" << solve("input.txt");

    return 0;
}
