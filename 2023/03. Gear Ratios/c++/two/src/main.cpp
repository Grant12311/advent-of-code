#include <array>
#include <cctype>
#include <cstddef>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <numeric>
#include <string>
#include <unordered_map>
#include <vector>

/**
 * @brief Add the part number found at x, y in grid into partNumbers if present
 * 
 * @param grid 
 * @param partNumbers 
 * @param x 
 * @param y 
 */
static void processNeighbor(const std::vector<std::vector<char>>& grid, std::unordered_map<const char*, int>& partNumbers, const std::size_t x, const std::size_t y)
{
    // No needed to check less than zero because std::size_t is unsigned
    if (y >= grid.size() || x >= grid[y].size())
        return;

    if (std::isdigit(grid[y][x]))
    {
        // Find start of number
        auto first{grid[y].begin() + static_cast<std::ptrdiff_t>(x)};
        while (first != grid[y].begin() && std::isdigit(*std::prev(first)))
        {
            --first;
        }

        // Find end of number
        auto last{grid[y].begin() + static_cast<std::ptrdiff_t>(x)};
        while (last != std::prev(grid[y].end()) && std::isdigit(*std::next(last)))
        {
            ++last;
        }

        // Parse number and add to map
        const int partNumber{std::stoi(std::string{first, last + 1})};
        partNumbers.insert({first.base(), partNumber});
    }
}

int main(const int argc, const char** const argv)
{
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " <INPUT_FILE>\n";
        return 1;
    }

    std::ifstream infile{argv[1]};
    if (!infile.is_open())
    {
        std::cerr << "ERROR! Could not open input file " << std::quoted(argv[1]) << '\n';
        return 1;
    }

    // Puzzle result
    int partNumberSum{};

    // Store the grid loaded from the file
    std::vector<std::vector<char>> grid;

    // Read each line
    for (std::string line; std::getline(infile, line);)
    {
        grid.emplace_back(line.begin(), line.end());
    }

    // Iterate over all tiles looking for stars
    for (std::size_t y{}; y < grid.size(); ++y)
    {
        for (std::size_t x{}; x < grid[y].size(); ++x)
        {
            const char c{grid[y][x]};

            // Check all neighbors
            if (c == '*')
            {
                std::unordered_map<const char*, int> partNumbers;

                processNeighbor(grid, partNumbers, x, y - 1); // North
                processNeighbor(grid, partNumbers, x, y + 1); // South
                processNeighbor(grid, partNumbers, x + 1, y); // East
                processNeighbor(grid, partNumbers, x - 1, y); // West
                processNeighbor(grid, partNumbers, x + 1, y - 1); // North-East
                processNeighbor(grid, partNumbers, x + 1, y + 1); // South-East
                processNeighbor(grid, partNumbers, x - 1, y + 1); // South-West
                processNeighbor(grid, partNumbers, x - 1, y - 1); // North-West

                // If c has exactly 2 neighbors add both those numbers to sum
                if (partNumbers.size() == 2)
                    partNumberSum += partNumbers.begin()->second * std::next(partNumbers.begin())->second;
            }
        }
    }

    std::cout << "Result: " << partNumberSum << '\n';

    return 0;
}