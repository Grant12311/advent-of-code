#include <fstream>
#include <iostream>
#include <limits>

int main()
{
    std::ifstream infile{"input.txt"};

    if (!infile.is_open())
    {
        std::cerr << "ERROR! Unable to open input file!\n";
        return -1;
    }

    int increases{};

    int prev{std::numeric_limits<int>::max()};
    int curr;

    while (infile >> curr)
    {
        if (curr > prev)
            ++increases;

        prev = curr;
    }

    std::cout << "Answer: " << increases << '\n';

    return 0;
}