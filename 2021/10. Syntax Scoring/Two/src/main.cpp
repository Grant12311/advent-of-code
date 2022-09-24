#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <stack>
#include <string>
#include <vector>

enum Score
{
    PAREN = 1,
    SQUARE_BRACKET,
    CURLY_BRACKET,
    ANGLED_BRACKET
};

int main()
{
    std::ifstream infile{"input.txt"};

    if (!infile.is_open())
    {
        std::cerr << "ERROR! Could not open file!\n";
        return EXIT_FAILURE;
    }

    // Score of each line
    std::vector<std::uint64_t> scores;

    // Read file line by line
    std::string line;
    while (std::getline(infile, line))
    {
        std::stack<char> stack;

        // Set to true if line is corrupted
        bool fail{false};

        // Read each character from line and stop if the line is corrupted
        for (std::size_t i{}; i < line.size() && !fail; ++i)
        {
            const char c{line[i]};

            switch (c)
            {
                // Push to stack if left
                case '(':
                case '[':
                case '{':
                case '<':
                    stack.push(c);
                    break;

                // If right check for matching left on top to see if string is corrupted 

                case ')':
                    if (stack.top() == '(')
                    {
                        stack.pop();
                    }
                    else
                    {
                        fail = true;
                    }
                    break;
                case ']':
                    if (stack.top() == '[')
                    {
                        stack.pop();
                    }
                    else
                    {
                        fail = true;
                    }
                    break;
                case '}':
                    if (stack.top() == '{')
                    {
                        stack.pop();
                    }
                    else
                    {
                        fail = true;
                    }
                    break;
                case '>':
                    if (stack.top() == '<')
                    {
                        stack.pop();
                    }
                    else
                    {
                        fail = true;
                    }
                    break;
            }
        }

        // If corrupted skip to next line
        if (fail)
            continue;

        std::uint64_t score{};

        // For each leftover left bracket on stack add value of match to score
        while (!stack.empty())
        {
            const char brack{stack.top()};
            stack.pop();

            score *= 5;

            switch (brack)
            {
                case '(':
                    score += Score::PAREN;
                    break;
                case '[':
                    score += Score::SQUARE_BRACKET;
                    break;
                case '{':
                    score += Score::CURLY_BRACKET;
                    break;
                case '<':
                    score += Score::ANGLED_BRACKET;
                    break;
            }
        }

        // Record line score in vector
        scores.push_back(score);
    }

    // Sort scores and use median as answer
    // Guaranteed odd number of scores by rules

    std::sort(scores.begin(), scores.end());

    std::cout << "Answer: " << scores[scores.size() / 2] << '\n';

    return EXIT_SUCCESS;
}