#include <fstream>
#include <iostream>

int main()
{
    std::ifstream infile{"input.txt"};

    int position{};
    int depth{};

    std::string command;
    int value;

    while (infile >> command >> value)
    {
        if (command == "forward")
            position += value;
        else if (command == "up")
            depth -= value;
        else
            depth += value;
    }

    std::cout << "Answer: " << position * depth << '\n';

    return 0;
}