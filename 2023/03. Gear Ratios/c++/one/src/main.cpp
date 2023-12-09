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

    // Map of (pointer to start of number, numeric value)
    // Used to prevent the same instance of a number from being added multiple times while still allow the same numeric value to be duplicated
    std::unordered_map<char*, int> partNumbers;

    // Store the grid loaded from the file
    std::vector<std::vector<char>> grid;

    // Lambda to check if a given tile is part of a number and then add that number to partNumbers
    const auto processNeighbor{[&partNumbers, &grid](const std::size_t x, const std::size_t y)
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
    }};

    // Read each line
    for (std::string line; std::getline(infile, line);)
    {
        grid.emplace_back(line.begin(), line.end());
    }

    // Iterate over all tiles looking for symbols
    for (std::size_t y{}; y < grid.size(); ++y)
    {
        for (std::size_t x{}; x < grid[y].size(); ++x)
        {
            const char c{grid[y][x]};

            if (c != '.' && std::ispunct(c))
            {
                processNeighbor(x, y - 1); // North
                processNeighbor(x, y + 1); // South
                processNeighbor(x + 1, y); // East
                processNeighbor(x - 1, y); // West
                processNeighbor(x + 1, y - 1); // North-East
                processNeighbor(x + 1, y + 1); // South-East
                processNeighbor(x - 1, y + 1); // South-West
                processNeighbor(x - 1, y - 1); // North-West
            }
        }
    }

    // Sum part numbers
    int partNumberSum{};
    for (const auto&[_, partNumber] : partNumbers)
    {
        partNumberSum += partNumber;
    }

    std::cout << "Result: " << partNumberSum << '\n';

    return 0;
}