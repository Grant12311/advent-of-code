#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <limits>
#include <string>
#include <vector>

[[nodiscard]]
static int calcFuelCostOfMoves(const int moves) noexcept
{
    return moves * (moves + 1) / 2;
}

[[nodiscard]]
static int calcFuelCostOfPoint(const std::vector<int>& crabs, const int point) noexcept
{
    int result{};

    for (const int position : crabs)
    {
        result += calcFuelCostOfMoves(std::abs(point - position));
    }

    return result;
}

int main()
{
    std::ifstream infile{"input.txt"};

    std::vector<int> crabs;

    std::string line;
    while (std::getline(infile, line, ','))
    {
        const int position{std::stoi(line)};
        crabs.push_back(position);
    }

    int answer{std::numeric_limits<int>::max()};

    for (const int position : crabs)
    {
        answer = std::min(answer, calcFuelCostOfPoint(crabs, position));
    }

    std::cout << "Answer: " << answer << '\n';

    return 0;
}