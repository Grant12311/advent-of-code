#include <cstddef>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <vector>

struct Race
{
    long long int time{};
    long long int distance{};
};

/**
 * @brief Insert a series of time values from infile in the format of "Time: 7 15 30" into races
 *        Only the "time" fields of each race will be set. Distances will all be zero
 *        std::runtime error will be thrown if infile does not start with a string
 * 
 * @param infile 
 * @param races 
 */
static void readTimes(std::ifstream& infile, std::vector<Race>& races)
{
    std::string str;
    if (!(infile >> str))
        throw std::runtime_error{"Time data in invalid"};

    for (long long int time; infile >> time;)
    {
        races.emplace_back(time);
    }
    infile.clear();
}

/**
 * @brief Read a series of distance values from infile in the format of "Time: 7 15 30" into races
 *        Values will be read until either input runs out of distances or races runs out of races
 *        std::runtime error will be thrown if infile does not start with a string
 * 
 * @param infile 
 * @param races 
 */
static void readDistances(std::ifstream& infile, std::vector<Race>& races)
{
    std::string str;
    if (!(infile >> str))
        throw std::runtime_error{"Distance data in invalid"};

    for (std::size_t i{}; i < races.size() && infile >> races[i].distance; ++i);
    infile.clear();
}

/**
 * @brief Read race time and distance values from infile in the format of
 *        Time:      7  15   30
*         Distance:  9  40  200
 * 
 * @param infile 
 * @return std::vector<Race> 
 */
[[nodiscard]]
static std::vector<Race> readRaces(std::ifstream& infile)
{
    std::vector<Race> races;

    readTimes(infile, races);
    readDistances(infile, races);

    return races;
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

    std::vector<Race> races{readRaces(infile)};
    infile.close();

    // Product of all the win method counts
    unsigned long long int result{1};

    for (const Race& race : races)
    {
        result *= winMethods(race);
    }

    std::cout << "Result: " << result << '\n';

    return 0;
}