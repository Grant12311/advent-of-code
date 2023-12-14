#include <algorithm>
#include <cstddef>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <string>
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
        std::cerr << "ERROR! Failed to open " << std::quoted(argv[1]) << '\n';
        return 1;
    }

    // Puzzle result
    int cardCount{};

    // Numbers found and winning numbers respectively. Both cleared after each iteration
    std::vector<int> presentNumbers;
    std::vector<int> winningNumbers;

    // Number of extra copies to add for the next card is present in front
    std::vector<int> copies;

    std::size_t lineNum{1};
    for (std::string line; std::getline(infile, line); ++lineNum, presentNumbers.clear(), winningNumbers.clear())
    {
        std::istringstream ss{line};
        
        // Check for "Card" string
        ss >> line;
        if (line != "Card")
        {
            std::cerr << "Line " << lineNum << " is invalied\n";
            return 1;
        }

        // Check card number matches expected
        int cardNum;
        ss >> cardNum;
        if (cardNum != static_cast<int>(lineNum))
        {
            std::cerr << "Invalid card number on line " << lineNum << '\n';
            return 1;
        }

        // Check for colon to mark start of present numbers
        if (ss.get() != ':')
        {
            std::cerr << "Expected colon on line " << lineNum << '\n';
            return 1;
        }

        // Read all present numbers. '|' triggers failure that will be cleared after loop
        for (int number; ss >> number;)
        {
            presentNumbers.push_back(number);
        }
        ss.clear();

        // Find and remove pipe
        ss.ignore(std::numeric_limits<std::streamsize>::max(), '|');

        // Read all winning numbers
        for (int number; ss >> number;)
        {
            winningNumbers.push_back(number);
        }

        // Sort for binary search for fast lookup
        std::ranges::sort(winningNumbers);

        // Count wins of card
        std::size_t wins{};
        for (const int presentNumber : presentNumbers)
        {
            if (std::ranges::binary_search(winningNumbers, presentNumber))
                ++wins;
        };

        // Add extra copies of necessary
        int copiesCurrent{1};
        if (!copies.empty())
        {
            copiesCurrent += copies.front();
            copies.erase(copies.begin());
        }

        // Add copies to total card count
        cardCount += copiesCurrent;

        // Resize copies buffer to ensure enough room them increment copy counts
        copies.resize(std::max(wins, copies.size()));
        for (std::size_t i{}; i < wins; ++i)
        {
            copies[i] += copiesCurrent;
        }
    }

    std::cout << "Result: " << cardCount << '\n';

    return 0;
}