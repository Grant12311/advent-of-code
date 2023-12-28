#include <cctype>
#include <cstddef>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <vector>

struct Race
{
    long long int time{};
    long long int distance{};
};

/**
 * @brief Converts string containing digits, combines all digits into one string, and converts that to a number with std::stoll
 * 
 * @param str 
 * @return long long int 
 */
long long int combineNumberPortions(const std::string& str)
{
    std::string combinded;
    for (const char c : str)
    {
        if (std::isdigit(c))
            combinded.push_back(c);
    }

    return std::stoll(combinded);
}

/**
 * @brief Calcualte the number of possible ways to win race
 * 
 * @param race 
 * @return unsigned long long int 
 */
[[nodiscard]]
static unsigned long long int winMethods(const Race& race)
{
    unsigned long long int methods{};

    // After the first win, any loss means we can't hold for any longer and still win, so abort
    bool firstWin{false};
    for (long long int hold{1}; hold <= race.time; ++hold)
    {
        if ((race.time - hold) * hold > race.distance)
        {
            firstWin = true;
            ++methods;
        }
        else if (firstWin)
        {
            break;
        }
    }

    return methods;
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
        std::cerr << "ERROR! Failed to open " << std::quoted(argv[1]) << '\n';
        return 1;
    }

    std::string start;
    std::string numbers;

    Race race;

    // Read times
    if (!(infile >> start) || start != "Time:")
    {
        std::cerr << "ERROR! Failed to read start of times\n";
        return 1;
    }

    if (!std::getline(infile, numbers))
    {
        std::cerr << "ERROR! Failed to read times\n";
        return 1;
    }

    race.time = combineNumberPortions(numbers);

    // Read distances
    if (!(infile >> start) || start != "Distance:")
    {
        std::cerr << "ERROR! Failed to read start of distances\n";
        return 1;
    }

    if (!std::getline(infile, numbers))
    {
        std::cerr << "ERROR! Failed to read distances\n";
        return 1;
    }

    race.distance = combineNumberPortions(numbers);

    std::cout << "Result: " << winMethods(race) << '\n';

    return 0;
}