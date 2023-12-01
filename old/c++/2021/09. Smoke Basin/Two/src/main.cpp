#include <algorithm>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <stack>
#include <string>
#include <vector>

struct Coordinate
{
    std::size_t x{};
    std::size_t y{};
};

static void basinSize(const std::vector<std::vector<int>>& map, const std::size_t x, const std::size_t y, std::vector<std::vector<bool>>& visited, int& result)
{
    if (visited[y][x])
        return;

    ++result;
    visited[y][x] = true;

    const int value{map[y][x]};

    const bool hasNorthNeighbor{y > 0};
    const bool hasSouthNeighbor{y < map.size() - 1};
    const bool hasEastNeighbor{x < map[y].size() - 1};
    const bool hasWestNeighbor{x > 0};

    // North
    if (hasNorthNeighbor && value < map[y - 1][x] && map[y - 1][x] != 9)
        basinSize(map, x, y - 1, visited, result);
    // South
    if (hasSouthNeighbor && value < map[y + 1][x] && map[y + 1][x] != 9)
        basinSize(map, x, y + 1, visited, result);
    // East
    if (hasEastNeighbor && value < map[y][x + 1] && map[y][x + 1] != 9)
        basinSize(map, x + 1, y, visited, result);
    // West
    if (hasWestNeighbor && value < map[y][x - 1] && map[y][x - 1] != 9)
        basinSize(map, x - 1, y, visited, result);
}

[[nodiscard]]
static int basinSize(const std::vector<std::vector<int>>& map, const std::size_t x, const std::size_t y)
{
    int result{0};

    std::vector<std::vector<bool>> visited(map.size());
    for (std::size_t i{}; i < map.size(); ++i)
    {
        visited[i].resize(map[i].size());
    }

    basinSize(map, x, y, visited, result);

    return result;
}

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

    std::vector<int> basins;

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
                basins.push_back(basinSize(map, x, y));
        }
    }

    std::sort(basins.rbegin(), basins.rend());

    int answer{1};

    for (std::size_t i{}; i < 3 && i < basins.size(); ++i)
    {
        answer *= basins[i];
    }

    std::cout << "Answer: " << answer << '\n';

    return 0;
}