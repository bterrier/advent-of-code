#include <cinttypes>

#include <algorithm>
#include <functional>
#include <iostream>
#include <numeric>
#include <ranges>
#include <utility>
#include <vector>

using Item = std::uint64_t;
using MonkeyId = std::size_t;
extern const Item md;
constexpr auto MONKEY_COUNT = 8;

class Queue
{
public:
     Queue() = default;
     explicit Queue(std::initializer_list<Item> items) :
        m_queueLength(items.size())
    {
        std::ranges::copy(items, m_array.begin());
    }

     void clear()
    {
        m_queueLength = 0;
    }

     auto size() const
    {
        return m_queueLength;
    }

     Item at(size_t index) const
    {
        return m_array.at(index);
    }

     void append(Item item)
    {
        m_array[m_queueLength] = item;
        ++m_queueLength;
    }
private:
    std::array<Item, 256> m_array = { 0 };
    std::size_t m_queueLength = 0;
};

class Monkey
{
public:
    template <typename Op, typename Test>
     Monkey(std::initializer_list<Item> items,
                     Op &&op,
                     Test &&test,
                     MonkeyId monkeyTrue,
                     MonkeyId monkeyFalse
                     ) :
        m_items(items),
        m_op(std::forward<Op>(op)),
        m_test(std::forward<Test>(test)),
        m_monkeyTrue(monkeyTrue),
        m_monkeyFalse(monkeyFalse)
    {}

     void run()
    {
        for (std::size_t i = 0 ; i < m_items.size() ; ++i) {
            auto item = m_items.at(i);
            item = m_op(item);
            item %= md;
            if (m_test(item)) {
                s_monkeys[m_monkeyTrue].m_items.append(item);
            } else {
                s_monkeys[m_monkeyFalse].m_items.append(item);
            }
            m_count += 1;
        }

        m_items.clear();
    }
    static  void round()
    {
        for (Monkey &monkey: s_monkeys){
            monkey.run();
        }
    }

    static  std::uint64_t result(){
        std::array<std::uint64_t, MONKEY_COUNT> items;
        std::ranges::transform(s_monkeys, items.begin(), [](const Monkey &m){return m.m_count;});
        std::ranges::sort(items, std::greater());
        return items[0] * items[1];
    }
private:
    Queue m_items;
    using Op = Item(Item);
    Op * const m_op;
    using Test = bool(Item);
    Test * const m_test;
    const MonkeyId m_monkeyTrue;
    const MonkeyId m_monkeyFalse;
    static std::array<Monkey, MONKEY_COUNT> s_monkeys;
    std::uint64_t m_count = 0;
};

constexpr auto lcm(auto x, auto y) {
    return std::lcm(x,y);
}
constexpr auto lcm(auto head, auto...tail) {
    return std::lcm(head, lcm(tail...));
}

#if 1

constexpr Item md = lcm(3, 5, 2,13,11,17,19,7);
std::array<Monkey, MONKEY_COUNT> Monkey::s_monkeys = {
    Monkey{
        {99, 67, 92, 61, 83, 64, 98},
        [](Item old)  { return old * 17; },
        [](Item old)  { return old % 3 == 0; },
        4,
        2
    },

    Monkey {
        {78, 74, 88, 89, 50},
        [](Item old)  {return old * 11;},
        [](Item old)  { return old % 5 == 0; },
        3,
        5
    },

    Monkey {
        {98, 91},
        [](Item old)  {return old + 4;},
        [](Item old)  { return old % 2 == 0; },
        6,
        4
    },

    Monkey {
        {59, 72, 94, 91, 79, 88, 94, 51},
        [](Item old)  {return old * old; },
        [](Item old)  { return old % 13 == 0; },
        0,
        5
    },



    Monkey{
        {95, 72, 78},
        [](Item old)  { return old + 7; },
        [](Item old)  { return old % 11 == 0; },
        7,
        6
    },

    Monkey {
        {76},
        [](Item old)  {return old + 8;},
        [](Item old)  { return old % 17 == 0; },
        0,
        2
    },

    Monkey {
        {69, 60, 53, 89, 71, 88},
        [](Item old)  {return old + 5;},
        [](Item old)  { return old % 19 == 0; },
        7,
        1
    },

    Monkey {
        {72, 54, 63, 80},
        [](Item old)  {return old + 3; },
        [](Item old)  { return old % 7 == 0; },
        1,
        3
    },
};
#else

constexpr Item md = lcm(23,19,13,17);
std::array<Monkey, MONKEY_COUNT> Monkey::s_monkeys = {
    Monkey{
        {79, 98},
        [](Item old)  { return old * 19; },
        [](Item old)  { return old % 23 == 0; },
        2,
        3
    },

    Monkey {
        {54, 65, 75, 74},
        [](Item old)  {return old + 6;},
        [](Item old)  { return old % 19 == 0; },
        2,
        0
    },

    Monkey {
        { 79, 60, 97},
        [](Item old)  {return old * old;},

        [](Item old)  { return old % 13 == 0; },
        1,
        3
    },

    Monkey {
        {74},
        [](Item old)  {return old + 3; },
        [](Item old)  { return old % 17 == 0; },
        0,
        1
    },

    };
#endif

int main()
{

    for (int i = 0 ; i < 10000 ; ++i) {
        std::cout << i << std::endl;
        Monkey::round();
    }

    std::cout << Monkey::result() << std::endl;
    return 0;
}

