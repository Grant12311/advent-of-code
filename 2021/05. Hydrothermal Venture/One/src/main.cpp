#include <algorithm>
#include <cassert>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <utility>

struct Point
{
    int x{};
    int y{};

    bool operator==(const Point& rhs) const = default;
};

template<>
struct std::hash<Point>
{
    std::size_t operator()(const Point& point) const noexcept
    {
        const std::size_t h1{std::hash<int>{}(point.x)};
        const std::size_t h2{std::hash<int>{}(point.y)};
        return h1 ^ (h2 << 1);
    }
};

int main()
{
    using namespace std::string_literals;

    std::ifstream infile{"input.txt"};

    std::unordered_map<Point, int> counts;

    std::string line;
    while (std::getline(infile, line))
    {
        const std::size_t comma1Pos{line.find(',')};
        const std::size_t comma2Pos{line.find_last_of(',')};
        const std::size_t firstSpacePos{line.find(' ')};
        const std::size_t lastSpacePos{line.find('>') + 1};

        const int x1{std::stoi(line.substr(0, comma1Pos))};
        const int y1{std::stoi(line.substr(comma1Pos + 1, firstSpacePos - comma1Pos + 1))};

        const int x2{std::stoi(line.substr(lastSpacePos + 1, comma2Pos - (lastSpacePos + 1)))};
        const int y2{std::stoi(line.substr(comma2Pos + 1))};

        if (x1 != x2 && y1 != y2)
            continue;

        int x{x1};
        int y{y1};

        ++counts[{x, y}];

        do
        {
            if (x < x2)
                ++x;
            else if (x > x2)
                --x;

            if (y < y2)
                ++y;
            else if (y > y2)
                --y;

            ++counts[{x, y}];
        } while (x != x2 || y != y2);
    }

    int answer{};

    for (const auto&[point, count] : counts)
    {
        if (count >= 2)
        {
            ++answer;
        }
    }

    std::cout << "Answer: " << answer << '\n';

    return 0;
}