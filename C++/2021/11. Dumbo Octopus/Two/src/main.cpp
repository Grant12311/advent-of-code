#include <cstddef>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

struct Octopus
{
    int energy{};
    bool flashed{};
};

class Map
{
public:
    void addRow(const std::string& line)
    {
        std::vector<Octopus>& row{map.emplace_back(line.size())};

        for (std::size_t i{}; i < line.size(); ++i)
        {
            row[i].energy = line[i] - '0';
        }
    }

    void resetStatus() noexcept
    {
        this->m_flashesThisStep = 0;

        for (std::size_t y{}; y < this->map.size(); ++y)
        {
            for (std::size_t x{}; x < this->map[y].size(); ++x)
            {
                if (map[y][x].flashed)
                {
                    map[y][x].energy = 0;
                    map[y][x].flashed = false;
                }
            }
        }
    }

    [[nodiscard]]
    bool stepDone() const noexcept
    {
        for (std::size_t y{}; y < this->map.size(); ++y)
        {
            for (std::size_t x{}; x < this->map[y].size(); ++x)
            {
                if (map[y][x].energy > 9 && !map[y][x].flashed)
                    return false;
            }
        }

        return true;
    }

    void flash(const std::size_t x, const std::size_t y)
    {
        ++this->m_flashesThisStep;

        const bool hasNorthNeighbor{y > 0};
        const bool hasSouthNeighbor{y < this->map.size() - 1};
        const bool hasEastNeighbor{x < this->map[0].size() - 1};
        const bool hasWestNeighbor{x > 0};

        const bool hasNorthEastNeighbor{hasNorthNeighbor && hasEastNeighbor};
        const bool hasNorthWestNeighbor{hasNorthNeighbor && hasWestNeighbor};
        const bool hasSouthEastNeighbor{hasSouthNeighbor && hasEastNeighbor};
        const bool hasSouthWestNeighbor{hasSouthNeighbor && hasWestNeighbor};

        if (hasNorthNeighbor)
            ++this->map[y - 1][x].energy;
        if (hasSouthNeighbor)
            ++this->map[y + 1][x].energy;
        if (hasEastNeighbor)
            ++this->map[y][x + 1].energy;
        if (hasWestNeighbor)
            ++this->map[y][x - 1].energy;

        if (hasNorthEastNeighbor)
            ++this->map[y - 1][x + 1].energy;
        if (hasNorthWestNeighbor)
            ++this->map[y - 1][x - 1].energy;
        if (hasSouthEastNeighbor)
            ++this->map[y + 1][x + 1].energy;
        if (hasSouthWestNeighbor)
            ++this->map[y + 1][x - 1].energy;

        this->map[y][x].flashed = true;
    }

    [[nodiscard]]
    constexpr int flashesThisStep() const noexcept
    {
        return this->m_flashesThisStep;
    }

    [[nodiscard]]
    std::size_t size() const noexcept
    {
        return this->map.size();
    }

    [[nodiscard]]
    int area() const noexcept
    {
        int result{};

        for (std::size_t y{}; y < this->map.size(); ++y)
        {
            for (std::size_t x{}; x < this->map[y].size(); ++x)
            {
                ++result;
            }
        }

        return result;
    }

    [[nodiscard]]
    std::vector<Octopus>& operator[](const std::size_t y)
    {
        return this->map[y];
    }
private:
    std::vector<std::vector<Octopus>> map;
    int m_flashesThisStep{};
};

int main()
{
    std::ifstream infile{"input.txt"};

    if (!infile.is_open())
    {
        std::cerr << "ERROR! Could not open file!\n";
        return -1;
    }

    Map map;

    // Load input file

    std::string line;
    while (std::getline(infile, line))
    {
        map.addRow(line);
    }

    // Main loop

    int step{};

    for (; map.flashesThisStep() != map.area(); ++step)
    {
        // Reset energy of octopuses that flashed
        map.resetStatus();
        
        // Increase energy level of each octopus

        for (std::size_t y{}; y < map.size(); ++y)
        {
            for (std::size_t x{}; x < map[y].size(); ++x)
            {
                ++map[y][x].energy;
            }
        }

        // Trigger flashes

        while (!map.stepDone())
        {
            for (std::size_t y{}; y < map.size(); ++y)
            {
                for (std::size_t x{}; x < map[y].size(); ++x)
                {
                    if (map[y][x].energy > 9 && !map[y][x].flashed)
                    {
                        map.flash(x, y);
                    }
                }
            }
        }
    }

    std::cout << "Answer: " << step << '\n';

    return 0;
}