#include <fstream>
#include <iostream>

int main()
{
    std::ifstream infile("input.txt");

    if (!infile.is_open())
    {
        std::cerr << "ERROR! Could not open input.txt!\n";
        return -1;
    }

    int prev = 0;
    int current = 0;

    int increases = 0;

    infile >> prev;

    while (infile >> current)
    {
        if (current > prev)
            increases++;

        prev = current;
    }

    std::cout << "Answer: " << increases << '\n';

    return 0;
}
