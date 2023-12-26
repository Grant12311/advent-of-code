#include <algorithm>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

/**
 * @brief Read list of seed numbers
 * 
 * @param infile 
 * @return std::vector<unsigned long long int> 
 */
[[nodiscard]]
static std::vector<unsigned long long int> readSeeds(std::istream& infile)
{
    std::vector<unsigned long long int> seeds;

    std::string str;
    std::getline(infile, str);

    std::istringstream ss{str};

    // Read and verify seed line start
    ss >> str;
    if (str != "seeds:")
    {
        return {};
    }

    // Read actual seed numbers
    for (unsigned long long int seed; ss >> seed;)
    {
        seeds.push_back(seed);
    }

    return seeds;
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
        std::cerr << "ERROR! Failed to open input file\n";
        return 1;
    }

    std::vector<unsigned long long int> seeds{readSeeds(infile)};

    // Flag to check if a seed at a given index has been converted yet by the current map. All set to false every iteration below
    std::vector<bool> seedConverted(seeds.size());

    for (std::string text; infile >> text;)
    {
        // Reset all conversion flags
        std::fill(seedConverted.begin(), seedConverted.end(), false);

        // Read and verify start of line
        infile >> text;
        if (text != "map:")
        {
            std::cerr << "ERROR! Invalid input\n";
            return 1;
        }

        // For each mapping
        unsigned long long int destStart;
        unsigned long long int srcStart;
        unsigned long long int size;
        while (infile >> destStart >> srcStart >> size)
        {
            // For each seed that's not been converted this iteration, convert if necessary
            for (std::size_t i{}; i < seeds.size(); ++i)
            {
                unsigned long long int& seed{seeds[i]};
                if (!seedConverted[i] && seed >= srcStart && seed < srcStart + size)
                {
                    seedConverted[i] = true;
                    seed = destStart + seed - srcStart;
                }
            }
        }

        // while loop will error when numbers run out
        infile.clear();
    }

    std::cout << "Result: " << *std::ranges::min_element(seeds) << '\n';

    return 0;
}