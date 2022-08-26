#include <array>
#include <fstream>
#include <iostream>
#include <numeric>

int main()
{
    std::ifstream infile{"input.txt"};

    std::array<int, 9> fish{};

    char input;
    while (infile.get(input))
    {
        ++fish[static_cast<std::size_t>(input - '0')];
        infile.ignore(1);
    }

    static constexpr int DAYS{80};

    for (int i{}; i < DAYS; ++i)
    {
        const int readyFish{fish[0]};
        fish[0] = 0;

        for (std::size_t j{1}; j < fish.size(); ++j)
        {
            fish[j - 1] = fish[j];
            fish[j] = 0;
        }

        fish[6] += readyFish;
        fish[8] += readyFish;
    }

    std::cout << "Answer: " << std::accumulate(fish.begin(), fish.end(), 0) << '\n';

    return 0;
}