#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string> 

// trim from start (in place)
static inline void ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](char ch) {
        return !std::isspace(ch);
    }));
}

// trim from end (in place)
static inline void rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](char ch) {
        return !std::isspace(ch);
    }).base(), s.end());
}

// trim from both ends (in place)
static inline void trim(std::string &s) {
    rtrim(s);
    ltrim(s);
}

int main()
{
    std::ifstream file("input.txt");

    std::string line;
    int max = 0;
    int sum = 0;
    while (std::getline(file, line))
    {
        trim(line);
        if (line.empty())
        {
            max = std::max(max, sum);
            sum = 0;
        }
        else
        {

            std::istringstream iss(line);
            int a;
            iss >> a;
            sum += a;
        }
    }

    max = std::max(max, sum);
    std::cout << max << std::endl;
    return 0;
}
