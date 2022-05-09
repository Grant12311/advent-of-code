#include <fstream>
#include <iostream>
#include <limits>

int main()
{
    std::ifstream infile{"input.txt"};

    if (!infile.is_open())
    {
        std::cerr << "ERROR! Could not open input file!\n";
        return -1;
    }

    int increases{-2};

    int prev2{};
    int prev1{};
    int curr;

    int lastSum{std::numeric_limits<int>::max()};

    while (infile >> curr)
    {
        const int sum = curr + prev1 + prev2;

        if (sum > lastSum)
            ++increases;

        prev2 = prev1;
        prev1 = curr;

        lastSum = sum;
    }

    std::cout << "Answer: " << increases << '\n';

    return 0;
}