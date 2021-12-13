#include <bitset>
#include <fstream>
#include <iostream>
#include <vector>

int main()
{
    constexpr size_t SIZE = 12;

    std::ifstream infile("input.txt");

    if (!infile.is_open())
    {
        std::cerr << "ERROR! Could not open input.txt!\n";
        return -1;
    }

    std::vector<std::bitset<SIZE>> data;

    std::bitset<SIZE> tmp;

    while (infile >> tmp)
    {
        data.emplace_back(tmp);
    }

    std::bitset<SIZE> gammaRate;

    for (size_t i = 0; i < SIZE; i++)
    {
        size_t ones = 0;
        size_t zeros = 0;

        for (size_t j = 0; j < data.size(); j++)
        {
            if (data[j][i])
                ones++;
            else
                zeros++;
        }

        gammaRate[i] = ones > zeros ? 1 : 0;
    }

    std::bitset<SIZE> epsilonRate;

    for (size_t i = 0; i < SIZE; i++)
    {
        size_t ones = 0;
        size_t zeros = 0;

        for (size_t j = 0; j < data.size(); j++)
        {
            if (data[j][i])
                ones++;
            else
                zeros++;
        }

        epsilonRate[i] = ones < zeros ? 1 : 0;
    }

    std::cout << "Answer: " << gammaRate.to_ulong() * epsilonRate.to_ulong() << '\n';

    return 0;
}
