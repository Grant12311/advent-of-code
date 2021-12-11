#include <cstddef>
#include <fstream>
#include <iostream>
#include <numeric>
#include <vector>

int main()
{
    std::ifstream infile("input.txt");

    if (!infile.is_open())
    {
        std::cerr << "ERROR! Could not open input.txt!\n";
        return -1;
    }

    std::vector<int> data;

    int tmp;

    while (infile >> tmp)
    {
        data.emplace_back(tmp);
    }

    int increases = 0;

    for (size_t i = 0, j = 1; i < data.size() - (data.size() % 3); i++, j++)
    {
        const int sum1 = std::accumulate(data.begin() + static_cast<long int>(i), data.begin()  + static_cast<long int>(i) + 3, 0);
        const int sum2 = std::accumulate(data.begin() + static_cast<long int>(j), data.begin()  + static_cast<long int>(j) + 3, 0);

        if (sum2 > sum1)
            increases++;
    }

    std::cout << "Answer: " << increases << '\n';

    return 0;
}
