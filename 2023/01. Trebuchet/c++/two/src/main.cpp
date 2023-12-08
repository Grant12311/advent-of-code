#include <algorithm>
#include <array>
#include <cstddef>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <ranges>
#include <string>
#include <string_view>

static constexpr std::array<std::string_view, 9> DIGIT_STRINGS{
    "one",
    "two",
    "three",
    "four",
    "five",
    "six",
    "seven",
    "eight",
    "nine"
};

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
        std::cerr << "ERROR! Failed to open " << std::quoted(argv[1]) << '\n';
        return 1;
    }

    int total{};

    for (std::string line; std::getline(infile, line);)
    {
        // Find first actual digit
        auto firstIt{std::ranges::find_if(line, [](const char c) -> bool
        {
            return std::isdigit(c);
        })};

        int first;

        // Search for every digit string and replace firstIt and set first if it's found closer to the begining
        for (std::size_t i{}; i < DIGIT_STRINGS.size(); ++i)
        {
            const auto digitString{DIGIT_STRINGS[i]};

            const std::size_t pos{line.find(digitString)};
            if (pos == std::string::npos)
                continue;

            const auto it{line.begin() + static_cast<std::ptrdiff_t>(pos)};

            if (it < firstIt)
            {
                firstIt = it;
                first = static_cast<int>(i) + 1;
            }
        }

        // Find last digit
        auto lastIt{std::ranges::find_if(std::views::reverse(line), [](const char c) -> bool
        {
            return std::isdigit(c);
        })};
        
        int last;

        // Search for every digit string and replace lastIt and set last if it's found closer to the begining
        for (std::size_t i{}; i < DIGIT_STRINGS.size(); ++i)
        {
            const auto digitString{DIGIT_STRINGS[i]};

            const std::size_t pos{line.rfind(digitString)};
            if (pos == std::string::npos)
                continue;

            // Extra math needed to convert from index to reverse iterator =
            const auto it{line.rbegin() + static_cast<std::ptrdiff_t>(line.size()) - 1 - static_cast<std::ptrdiff_t>(pos)};

            if (it < lastIt)
            {
                lastIt = it;
                last = static_cast<int>(i) + 1;
            }
        }

        // If the it's don't point to digits, they point to digit strings and the int representations has already been set
        if (std::isdigit(*firstIt))
            first = *firstIt - '0';
        if (std::isdigit(*lastIt))
            last = *lastIt - '0';

        // Assemble into full number and add to running total
        const int assembled{(first * 10) + last};
        total += assembled;
    }

    std::cout << "Result: " << total << '\n'; 

    return 0;
}