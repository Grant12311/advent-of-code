#include <array>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

static constexpr std::size_t SIZE{5};

struct Tile
{
    int value{};
    bool called{};
};

class Board
{
public:
    [[nodiscard]]
    constexpr int& tile(const std::size_t x, const std::size_t y) noexcept
    {
        return this->board[y][x].value;
    }

    [[nodiscard]]
    constexpr bool won() const noexcept
    {
        for (std::size_t x{}; x < SIZE; ++x)
        {
            bool allCalled{true};

            for (std::size_t y{}; y < SIZE; ++y)
            {
                if (!this->board[y][x].called)
                {
                    allCalled = false;
                    break;
                }
            }

            if (allCalled)
                return true;
        }

        for (std::size_t y{}; y < SIZE; ++y)
        {
            bool allCalled{true};

            for (std::size_t x{}; x < SIZE; ++x)
            {
                if (!this->board[y][x].called)
                {
                    allCalled = false;
                    break;
                }
            }

            if (allCalled)
                return true;
        }

        return false;
    }

    constexpr void call(const int number) noexcept
    {
        for (std::size_t y{}; y < SIZE; ++y)
        {
            for (std::size_t x{}; x < SIZE; ++x)
            {
                if (this->board[y][x].value == number)
                    this->board[y][x].called = true;
            }
        }
    }

    [[nodiscard]]
    int uncalledNumberSum() const
    {
        int result{};

        for (std::size_t y{}; y < SIZE; ++y)
        {
            for (std::size_t x{}; x < SIZE; ++x)
            {
                if (!this->board[y][x].called)
                    result += this->board[y][x].value;
            }
        }

        return result;
    }

    void print() const
    {
        for (std::size_t y{}; y < SIZE; ++y)
        {
            for (std::size_t x{}; x < SIZE; ++x)
            {
                std::cout << this->board[y][x].value << ' ';
            }
            std::cout << '\n';
        }

        std::cout << '\n';
    }
private:
    std::array<std::array<Tile, SIZE>, SIZE> board{};
};

[[nodiscard]]
static std::vector<int> loadNumberList(std::ifstream& infile)
{
    std::vector<int> result;

    std::string line;
    std::getline(infile, line);

    std::stringstream ss{line};

    while (std::getline(ss, line, ','))
    {
        result.push_back(std::stoi(line));
    }

    return result;
}

[[nodiscard]]
static std::vector<Board> loadBoards(std::ifstream& infile)
{
    std::vector<Board> result;

    int number;
    Board* board{&result.emplace_back()};
    std::size_t x{};
    std::size_t y{};

    while (infile >> number)
    {
        board->tile(x++, y) = number;

        if (x >= SIZE)
        {
            x = 0;
            ++y;
        }
        
        if (y >= SIZE)
        {
            y = 0;
            board = &result.emplace_back();
        }
    }

    return result;
}

int main()
{
    std::ifstream infile{"input.txt"};

    const std::vector<int> numberList{loadNumberList(infile)};

    std::vector<Board> boards{loadBoards(infile)};

    const Board* winningBoard{nullptr};
    int winningNumber;

    for (std::size_t i{}; i < numberList.size() && winningBoard == nullptr; ++i)
    {
        const int number{numberList[i]};

        for (Board& board : boards)
        {
            board.call(number);

            if (board.won())
            {
                winningBoard = &board;
                winningNumber = number;
                break;
            }
        }
    }

    // for (const Board& board : boards)
    // {
    //     board.print();
    // }

    std::cout << "Answer: " << winningBoard->uncalledNumberSum() * winningNumber << '\n';

    return 0;
}