#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

enum class Action
{
    Rock,
    Paper,
    Scissors
};

constexpr int action2points(Action a)
{
    switch (a)
    {
    case Action::Rock:
        return 1;

    case Action::Paper:
        return 2;
    case Action::Scissors:
        return 3;
    default:
        return 0;
    }
}

int fight(Action a, Action b)
{
    constexpr int WIN = 6;
    constexpr int DRAW = 3;
    constexpr int LOOSE = 0;

    if (a == b)
        return DRAW;
    
    switch (a)
    {
    case Action::Rock:
        switch (b)
        {
        case Action::Paper:
            return WIN;
        case Action::Scissors:
            return LOOSE;
        default:
            abort();
        }

    case Action::Paper:
        switch (b)
        {
        case Action::Rock:
            return LOOSE;
        case Action::Scissors:
            return WIN;
        default:
            abort();
        }

    case Action::Scissors:
        switch (b)
        {
        case Action::Rock:
            return WIN;
        case Action::Paper:
            return LOOSE;
        default:
            abort();
        }
    }

    abort();
}

constexpr Action char2action(char c)
{
    switch (c)
    {
    case 'A':
    case 'X':
        return Action::Rock;

    case 'B':
    case 'Y':
        return Action::Paper;

    case 'C':
    case 'Z':
        return Action::Scissors;

    default:
        abort();
    }
}

int main()
{

    std::ifstream file("input.txt");

    std::string line;

    int score = 0;
    while (std::getline(file, line))
    {
        auto a = char2action(line[0]);
        auto b = char2action(line[2]);
        score += action2points(b);
        score += fight(a, b);
    }

    std::cout << score << std::endl;
    return 0;
}
