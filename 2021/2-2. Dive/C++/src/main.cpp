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

    int position = 0;
    int depth = 0;
    int aim = 0;

    std::string type;
    int ammount;

    while (infile >> type >> ammount)
    {
        if (type == "forward")
        {
            position += ammount;
            depth += ammount * aim;
        }else if (type == "up"){
            aim -= ammount;
        }else{
            aim += ammount;
        }
    }

    std::cout << "Answer: " << position * depth << '\n';

    return 0;
}
