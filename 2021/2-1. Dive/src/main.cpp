#include <fstream>
#include <iostream>
#include <string>

int main()
{
    std::ifstream infile("input.txt");

    if (!infile.is_open())
    {
        std::cerr << "ERROR! Could not open input.txt!\n";
        return -1;
    }

    int x = 0;
    int depth = 0;

    std::string type;
    int ammount;

    while (infile >> type >> ammount)
    {
        if (type == "forward")
            x += ammount;
        else if (type == "up")
            depth -= ammount;
        else
            depth += ammount;
    }

    std::cout << "Answer: " << x * depth << '\n';

    return 0;
}
