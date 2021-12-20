#include <bitset>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <limits>
#include <optional>
#include <utility>
#include <vector>

template <size_t SIZE>
[[nodiscard]]
constexpr bool mostSignificatInBit(const std::vector<std::pair<std::bitset<SIZE>, bool>>& a_data, const size_t a_index) noexcept
{
    size_t ones = 0;
    size_t zeros = 0;

    for (size_t i = 0; i < a_data.size(); i++)
    {
        if (a_data[i].second)
        {
            if (a_data[i].first[a_index])
                ones++;
            else
                zeros++;
        }
    }

    return ones >= zeros ? 1 : 0;
}

template <size_t SIZE>
[[nodiscard]]
constexpr bool leastSignificatInBit(const std::vector<std::pair<std::bitset<SIZE>, bool>>& a_data, const size_t a_index) noexcept
{
    size_t ones = 0;
    size_t zeros = 0;

    for (size_t i = 0; i < a_data.size(); i++)
    {
        if (a_data[i].second)
        {
            if (a_data[i].first[a_index])
                ones++;
            else
                zeros++;
        }
    }

    return ones < zeros ? 1 : 0;
}

template <size_t SIZE>
[[nodiscard]]
constexpr size_t activeCount(const std::vector<std::pair<std::bitset<SIZE>, bool>>& a_data) noexcept
{
    size_t result = 0;

    for (size_t i = 0; i < a_data.size(); i++)
    {
        if (a_data[i].second)
            result++;
    }

    return result;
}

template <size_t SIZE>
constexpr void resetActiveCount(std::vector<std::pair<std::bitset<SIZE>, bool>>& a_data) noexcept
{
    for (size_t i = 0; i < a_data.size(); i++)
    {
        a_data[i].second = true;
    }
}

template <size_t SIZE>
constexpr std::optional<std::bitset<SIZE>> firstActiveValue(std::vector<std::pair<std::bitset<SIZE>, bool>>& a_data) noexcept
{
    for (size_t i = 0; i < a_data.size(); i++)
    {
        if (a_data[i].second)
            return a_data[i].first;
    }

    return {};
}

int main()
{
    constexpr size_t SIZE = 12;

    std::ifstream infile("input.txt");

    if (!infile.is_open())
    {
        std::cerr << "ERROR! Could not open input.txt!\n";
        return -1;
    }

    std::vector<std::pair<std::bitset<SIZE>, bool>> data;

    std::bitset<SIZE> tmp;

    while (infile >> tmp)
    {
        data.emplace_back(tmp, true);
    }

    for (size_t i = SIZE - 1; i != std::numeric_limits<size_t>::max() && activeCount(data) > 1; i--)
    {
        const bool sig = mostSignificatInBit(data, i);

        for (size_t j = 0; j < data.size(); j++)
        {
            if (data[j].first[i] != sig)
                data[j].second = false;
        }
    }

    const unsigned long int oxygen = firstActiveValue(data).value().to_ulong();

    resetActiveCount(data);

    for (size_t i = SIZE - 1; i != std::numeric_limits<size_t>::max() && activeCount(data) > 1; i--)
    {
        const bool least = leastSignificatInBit(data, i);

        for (size_t j = 0; j < data.size(); j++)
        {
            if (data[j].first[i] != least)
                data[j].second = false;
        }
    }


    const unsigned long int co2 = firstActiveValue(data).value().to_ulong();

    std::cout << "Answer: " << oxygen * co2 << '\n';

    return 0;
}
