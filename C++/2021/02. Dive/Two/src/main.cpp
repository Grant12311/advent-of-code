#include <fstream>
#include <iostream>

int main()
{
    std::ifstream infile{"input.txt"};

    int aim{};
    int position{};
    int depth{};

    std::string command;
    int value;

    while (infile >> command >> value)
    {
        if (command == "down")
        {
            aim += value;
        }
        else if (command == "up")
        {
            aim -= value;
        }
        else
        {
            position += value;
            depth += aim * value;
        }
    }

    std::cout << "Answer: " << position * depth << '\n';

    return 0;
}