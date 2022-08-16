#include <algorithm>
#include <array>
#include <bitset>
#include <cassert>
#include <fstream>
#include <iostream>
#include <iterator>
#include <limits>
#include <vector>

static constexpr std::size_t SIZE{12};

[[nodiscard]]
static std::vector<std::bitset<SIZE>> loadNumbers()
{
    std::vector<std::bitset<SIZE>> result;

    std::ifstream infile{"input.txt"};

    std::bitset<SIZE> number;
    while (infile >> number)
    {
        result.push_back(number);
    }

    return result;
}

int main()
{
    const std::vector<std::bitset<SIZE>> numbers{loadNumbers()};

    std::array<int, SIZE> netOnes{};

    std::vector<bool> isNumberRemaining(numbers.size(), true);
    std::size_t numbersRemaining{numbers.size()};

    for (std::size_t i{SIZE - 1}; i != std::numeric_limits<std::size_t>::max() && numbersRemaining > 1; --i)
    {
        for (std::size_t j{}; j < numbers.size(); ++j)
        {
            if (isNumberRemaining[j])
            {
                if (numbers[j][i])
                    ++netOnes[i];
                else
                    --netOnes[i];
            }
        }

        const bool wanted{netOnes[i] >= 0};

        for (std::size_t j{}; j < numbers.size() && numbersRemaining > 1; ++j)
        {
            if (isNumberRemaining[j] && numbers[j][i] != wanted)
            {
                isNumberRemaining[j] = false;
                --numbersRemaining;
            }
        }
    }

    const std::size_t oxygenRatingIndex{
        static_cast<std::size_t>(std::distance(isNumberRemaining.begin(), std::find(isNumberRemaining.begin(), isNumberRemaining.end(), true)))
    };

    std::fill(isNumberRemaining.begin(), isNumberRemaining.end(), true);
    std::fill(netOnes.begin(), netOnes.end(), 0);
    numbersRemaining = numbers.size();

    for (std::size_t i{SIZE - 1}; i != std::numeric_limits<std::size_t>::max() && numbersRemaining > 1; --i)
    {
        for (std::size_t j{}; j < numbers.size(); ++j)
        {
            if (isNumberRemaining[j])
            {
                if (numbers[j][i])
                    ++netOnes[i];
                else
                    --netOnes[i];
            }
        }

        const bool wanted{netOnes[i] < 0};

        for (std::size_t j{}; j < numbers.size() && numbersRemaining > 1; ++j)
        {
            if (isNumberRemaining[j] && numbers[j][i] != wanted)
            {
                isNumberRemaining[j] = false;
                --numbersRemaining;
            }
        }
    }

    assert(std::count(isNumberRemaining.begin(), isNumberRemaining.end(), true) == 1 && numbersRemaining == 1);

    const std::size_t co2RatingIndex{
        static_cast<std::size_t>(std::distance(isNumberRemaining.begin(), std::find(isNumberRemaining.begin(), isNumberRemaining.end(), true)))
    };

    std::cout << "Answer: " << numbers[oxygenRatingIndex].to_ullong() * numbers[co2RatingIndex].to_ullong() << '\n';

    return 0;
}