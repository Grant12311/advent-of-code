#include <cassert>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <stack>
#include <string>

static constexpr int PAREN_SCORE{3};
static constexpr int SQUARE_BRACKET_SCORE{57};
static constexpr int CURLY_BRACKET_SCORE{1197};
static constexpr int ANGLED_BRACKET_SCORE{25137};

int main()
{
    std::ifstream infile{"input.txt"};

    if (!infile.is_open())
    {
        std::cerr << "ERROR! Could not open file!\n";
        return EXIT_FAILURE;
    }

    int answer{};

    std::string line;
    while (std::getline(infile, line))
    {
        std::stack<char> stack;

        bool fail{false};

        for (std::size_t i{}; i < line.size() && !fail; ++i)
        {
            const char c{line[i]};

            switch (c)
            {
                case '(':
                case '[':
                case '{':
                case '<':
                    stack.push(c);
                    break;
                case ')':
                    if (stack.top() == '(')
                    {
                        stack.pop();
                    }
                    else
                    {
                        answer += PAREN_SCORE;
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
                        answer += SQUARE_BRACKET_SCORE;
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
                        answer += CURLY_BRACKET_SCORE;
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
                        answer += ANGLED_BRACKET_SCORE;
                        fail = true;
                    }
                    break;
            }
        }
    }

    std::cout << "Answer: " << answer << '\n';

    return EXIT_SUCCESS;
}