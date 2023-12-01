#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

[[nodiscard]]
static constexpr bool uniqueSegmentCount(const int count) noexcept
{
    return count == 2 || count == 4 || count == 3 || count == 7;
}

int main()
{
    std::ifstream infile{"input.txt"};

    int answer{};

    std::string line;
    while (std::getline(infile, line))
    {
        const std::size_t splitPos{line.find('|')};

        int count{};

        for (std::size_t i{splitPos + 2}; i < line.size(); ++i)
        {
            if (line[i] == ' ')
            {
                if (uniqueSegmentCount(count))
                    ++answer;
                    
                count = 0;
            }
            else
            {
                ++count;
            }
        }

        if (uniqueSegmentCount(count))
            ++answer;
    }

    std::cout << "Answer: " << answer << '\n';

    return 0;
}