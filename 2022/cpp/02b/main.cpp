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

Action actionForResult(Action opponent, char outcome)
{
    switch (outcome)
    {
    case 'Y':
        return opponent;

    case 'X':
        switch (opponent)
        {
        case Action::Rock:
            return Action::Scissors;
        case Action::Paper:
            return Action::Rock;
        case Action::Scissors:
            return Action::Paper;
        default:
            abort();
        }
    case 'Z':
        switch (opponent)
        {
        case Action::Rock:
            return Action::Paper;
        case Action::Paper:
            return Action::Scissors;
        case Action::Scissors:
            return Action::Rock;
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
        return Action::Rock;

    case 'B':
        return Action::Paper;

    case 'C':
        return Action::Scissors;

    default:
        abort();
    }
}

int main()
{
    constexpr int WIN = 6;
    constexpr int DRAW = 3;
    constexpr int LOOSE = 0;

    std::ifstream file("input.txt");

    std::string line;

    int score = 0;
    while (std::getline(file, line))
    {
        auto a = char2action(line[0]);
        auto result = line[2];
        Action b = actionForResult(a, result);
        score += action2points(b);

        switch (result)
        {
        case 'X':
            score += LOOSE;
            break;
        case 'Y':
            score += DRAW;
            break;
        case 'Z':
            score += WIN;
            break;
        }
    }

    std::cout << score << std::endl;
    return 0;
}
