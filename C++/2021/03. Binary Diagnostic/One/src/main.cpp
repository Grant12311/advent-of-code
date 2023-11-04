#include <array>
#include <bitset>
#include <fstream>
#include <iostream>
#include <utility>

int main()
{
    static constexpr std::size_t SIZE{12};
    
    std::ifstream infile{"input.txt"};

    std::bitset<SIZE> number;

    std::array<int, SIZE> netOnes{};

    while (infile >> number)
    {
        for (std::size_t i{}; i < number.size(); ++i)
        {
            if (number[i])
                ++netOnes[i];
            else
                --netOnes[i];
        }
    }

    std::bitset<SIZE> gammaRate;

    for (std::size_t i{}; i < netOnes.size(); ++i)
    {
        if (netOnes[i] > 0)
            gammaRate[i] = 1;
        else
            gammaRate[i] = 0;
    }

    std::bitset<SIZE> epsilionRate;

    for (std::size_t i{}; i < netOnes.size(); ++i)
    {
        if (netOnes[i] < 0)
            epsilionRate[i] = 1;
        else
            epsilionRate[i] = 0;
    }

    std::cout << gammaRate.to_ullong() * epsilionRate.to_ullong() << '\n';

    return 0;
}