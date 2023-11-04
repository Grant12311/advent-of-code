#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

int main()
{
    std::ifstream infile{"input.txt"};

    std::vector<int> crabs;

    std::string line;
    while (std::getline(infile, line, ','))
    {
        const int position{std::stoi(line)};
        crabs.push_back(position);
    }

    std::sort(crabs.begin(), crabs.end());

    const int median{crabs[crabs.size() / 2]};

    int answer{};

    for (const int position : crabs)
    {
        answer += std::abs(median - position);
    }

    std::cout << "Answer: " << answer << '\n';

    return 0;
}