#include <algorithm>
#include <cmath>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

struct SeedRange
{
    long long int start{};
    long long int size{};
};

/**
 * @brief Read list of seed ranges
 * 
 * @param infile 
 * @return std::vector<SeedRange>
 */
[[nodiscard]]
static std::vector<SeedRange> readSeeds(std::istream& infile)
{
    std::vector<SeedRange> seeds;

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
    for (long long int start, size; ss >> start >> size;)
    {
        seeds.emplace_back(start, size);
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

    std::vector<SeedRange> seeds{readSeeds(infile)};

    // Flags to check if a seed at a given index has been converted yet by the current map. All flags set to false at start of each map
    std::vector<bool> rangeConverted(seeds.size());

    // New breakaway ranges to insert into seeds after each seed is converted. Cleared after insertion
    std::vector<SeedRange> newSeeds{readSeeds(infile)};

    // For each mapping
    for (std::string text; infile >> text;)
    {
        // No seeds have been converted yet at the start of this mapping
        std::fill(rangeConverted.begin(), rangeConverted.end(), false);
        
        // Read and verify start of line
        infile >> text;
        if (text != "map:")
        {
            std::cerr << "ERROR! Invalid input\n";
            return 1;
        }

        // For each mapping in map
        long long int destStart;
        long long int srcStart;
        long long int size;
        while (infile >> destStart >> srcStart >> size)
        {
            // For each seed range that's not been converted this iteration, convert if necessary
            for (std::size_t i{}; i < seeds.size(); ++i)
            {
                SeedRange& startingRange{seeds[i]};

                // Skip already converted ranges
                if (rangeConverted[i])
                    continue;

                // If any part of this range should be converted
                if (startingRange.start + startingRange.size - 1 >= srcStart && startingRange.start < srcStart + size)
                {
                    // Break off portion before start of conversion area
                    if (startingRange.start < srcStart)
                    {
                        newSeeds.emplace_back(startingRange.start, srcStart - startingRange.start);
                        startingRange.size -= srcStart - startingRange.start;
                        startingRange.start = srcStart;
                    }

                    // Break off portion before after end of conversion area
                    if (startingRange.start + startingRange.size > srcStart + size)
                    {
                        newSeeds.emplace_back(srcStart + size, startingRange.start + startingRange.size - (srcStart + size));
                        startingRange.size -= startingRange.start + startingRange.size - (srcStart + size);
                    }

                    // Convert remaining portion
                    startingRange.start = destStart + startingRange.start - srcStart;
                    rangeConverted[i] = true;

                    // Insert any new seed ranges from above breakaway code
                    seeds.insert(seeds.end(), newSeeds.begin(), newSeeds.end());
                    newSeeds.clear();

                    // Resize to match current seed count
                    rangeConverted.resize(seeds.size());
                }
            }
        }

        // Map while loop will error when numbers run out
        infile.clear();
    }

    std::cout << "Result: " << std::ranges::min_element(seeds, std::less{}, &SeedRange::start)->start << '\n';

    return 0;
}