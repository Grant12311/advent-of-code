#include <algorithm>
#include <array>
#include <cstring>
#include <fstream>
#include <cstddef>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <string>
#include <string_view>
#include <vector>

static constexpr std::string_view DELIMS{" :;,"};
static const std::size_t NUM_COLORS{3};

static constexpr std::array<std::string_view, NUM_COLORS> COLOR_STRINGS{
    "red",
    "green",
    "blue"
};

static constexpr std::array<int, NUM_COLORS> MAX_CUBES{
    12, // red
    13, // green
    14  // blue
};

int main(const int argc, const char** const argv)
{
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " <INPUT_FILE\n>";
        return 1;
    }

    std::ifstream infile{argv[1]};
    if (!infile.is_open())
    {
        std::cerr << "ERROR! Could not open input file\n";
        return 1;
    }

    // Puzzle result
    int idSum{};

    // Read each line
    int lineNum{1};
    for (std::string line; std::getline(infile, line); ++lineNum)
    {
        // Minimum number of cubes of each color needed for current game to be possilbe
        std::array<int, NUM_COLORS> minNeeded{};

        // Copy line input mutable buffer for std::strtok
        std::vector<char> lineBuffer(line.size() + 1);
        std::copy(line.begin(), line.end(), lineBuffer.begin());

        // Skip first two tokens
        std::strtok(lineBuffer.data(), DELIMS.data());
        std::strtok(nullptr, DELIMS.data());

        // Read ammount, color pairs
        for (const char* ammountString{std::strtok(nullptr, DELIMS.data())}; ammountString != nullptr; ammountString = std::strtok(nullptr, DELIMS.data()))
        {
            const int ammount{std::stoi(ammountString)};
            const std::string_view colorString{std::strtok(nullptr, DELIMS.data())};

            // Get index for color string or print erorr message if color is not found
            const auto colorIt{std::ranges::find(COLOR_STRINGS, colorString)};
            if (colorIt == COLOR_STRINGS.end())
            {
                std::cerr << "Unknown color " << std::quoted(colorString) << " on line " << lineNum << '\n';
                return 1;
            }
            else
            {
                // If ammount is greater than current max for given color record new max
                const std::size_t colorIndex{static_cast<std::size_t>(std::distance(COLOR_STRINGS.begin(), colorIt))};
                minNeeded[colorIndex] = std::max(minNeeded[colorIndex], ammount);
            }
        }

        // Check if game is possible in regards to all 3 colors
        bool possible{true};
        for (std::size_t i{}; i < NUM_COLORS; ++i)
        {
            if (minNeeded[i] > MAX_CUBES[i])
            {
                possible = false;
                break;
            }
        }

        // Add lineId to total if game is possible
        if (possible)
            idSum += lineNum;
    }

    std::cout << "Result: " << idSum << '\n';

    return 0;
}