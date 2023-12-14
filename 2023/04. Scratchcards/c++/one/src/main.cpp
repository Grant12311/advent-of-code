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
    int scoreSum{};

    // Numbers found and winning numbers respectively. Both cleared after each iteration
    std::vector<int> presentNumbers;
    std::vector<int> winningNumbers;

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

        // Calculate score of card
        int score{};
        for (const int presentNumber : presentNumbers)
        {
            if (std::ranges::binary_search(winningNumbers, presentNumber))
                score = score == 0 ? 1 : score * 2;
        };

        scoreSum += score;
    }

    std::cout << "Result: " << scoreSum << '\n';

    return 0;
}