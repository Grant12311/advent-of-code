#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <ranges>
#include <string>

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
        // Find first digit and convert from char to int
        const int first{*std::ranges::find_if(line, [](const char c) -> bool
        {
            return std::isdigit(c);
        }) - '0'};

        // Find last digit and convert from char to int
        const int last{*std::ranges::find_if(std::views::reverse(line), [](const char c) -> bool
        {
            return std::isdigit(c);
        }) - '0'};

        // Assemble into full number and add to running total
        const int assembled{(first * 10) + last};
        total += assembled;
    }

    std::cout << "Result: " << total << '\n'; 

    return 0;
}