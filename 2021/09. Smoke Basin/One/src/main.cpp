#include <cstddef>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

int main()
{
    std::ifstream infile{"input.txt"};

    std::vector<std::vector<int>> map;

    std::string line;
    while (std::getline(infile, line))
    {
        std::vector<int>& row{map.emplace_back(line.size())};

        for (std::size_t i{}; i < line.size(); ++i)
        {
            row[i] = line[i] - '0';
        }
    }

    int answer{};

    for (std::size_t y{}; y < map.size(); ++y)
    {
        for (std::size_t x{}; x < map[y].size(); ++x)
        {
            const int value{map[y][x]};

            const bool hasNorthNeighbor{y > 0};
            const bool hasSouthNeighbor{y < map.size() - 1};
            const bool hasEastNeighbor{x < map[y].size() - 1};
            const bool hasWestNeighbor{x > 0};

            const bool lessThanNorth{!hasNorthNeighbor || value < map[y - 1][x]};
            const bool lessThanSouth{!hasSouthNeighbor || value < map[y + 1][x]};
            const bool lessThanEast{!hasEastNeighbor || value < map[y][x + 1]};
            const bool lessThanWest{!hasWestNeighbor || value < map[y][x - 1]};

            if (lessThanNorth && lessThanSouth && lessThanEast && lessThanWest)
                answer += value + 1;
        }
    }

    std::cout << "Answer: " << answer << '\n';

    return 0;
}