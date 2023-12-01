#include <algorithm>
#include <array>
#include <cassert>
#include <cctype>
#include <concepts>
#include <cmath>
#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>

enum class Segment
{
    A,
    B,
    C,
    D,
    E,
    F,
    G,
    COUNT
};

static constexpr Segment operator++(Segment& rhs) noexcept
{
    assert(rhs != Segment::COUNT);

    rhs = static_cast<Segment>(static_cast<int>(rhs) + 1);

    return rhs;
}

template <std::integral T>
static constexpr Segment operator+(const Segment lhs, const T rhs) noexcept
{
    return static_cast<Segment>(static_cast<T>(lhs) + rhs);
}

static constexpr std::size_t SEGMENT_COUNT{static_cast<std::size_t>(Segment::COUNT)};

[[nodiscard]]
constexpr static Segment charToSegment(const char c) noexcept
{
    return static_cast<Segment>(std::toupper(c) - 'A');
}

[[nodiscard]]
constexpr static char segmentToChar(const Segment seg) noexcept
{
    return 'A' + static_cast<char>(seg);
}

[[nodiscard]]
static int displayToInt(const std::string& display)
{
    switch (display.size())
    {
        case 2:
            assert(display == "CF");
            return 1;
            break;
        case 4:
            assert(display == "BCDF");
            return 4;
            break;
        case 3:
            assert(display == "ACF");
            return 7;
            break;
        case 7:
            assert(display == "ABCDEFG");
            return 8;
            break;
    }

    if (display == "ABCEFG")
        return 0;
    else if (display == "ACDEG")
        return 2;
    else if (display == "ACDFG")
        return 3;
    else if (display == "ABDFG")
        return 5;
    else if (display == "ABDEFG")
        return 6;
    else
        return 9;
}

[[nodiscard]]
static bool containsDuplicateCharacters(const std::string& str) noexcept
{
    std::array<int, 26> counts{};

    for (const char c : str)
    {
        ++counts[static_cast<std::size_t>(std::toupper(c) - 'A')];
    }

    for (const int count : counts)
    {
        if (count > 1)
            return true;
    }

    return false;
}

[[nodiscard]]
static bool containsDuplicateSegments(const std::array<Segment, SEGMENT_COUNT>& signal) noexcept
{
    std::array<int, SEGMENT_COUNT> counts{};

    for (const Segment seg : signal)
    {
        ++counts[static_cast<std::size_t>(seg)];
    }

    for (const int count : counts)
    {
        if (count > 1)
            return true;
    }

    return false;
}

[[nodiscard]]
static bool isValidSignal(const std::string& display)
{
    assert(std::is_sorted(display.begin(), display.end()));

    if (containsDuplicateCharacters(display))
        return false;

    return display == "ABCEFG" || display == "CF" || display == "ACDEG" || display == "ACDFG" || display == "BCDF" ||
        display == "ABDFG" || display == "ABDEFG" || display == "ACF" || display == "ABCDEFG" || display == "ABCDFG";
}

class SignalConverter
{
public:
    constexpr SignalConverter()
    {
        for (std::size_t i{}; i < this->table.size(); ++i)
        {
            std::fill(this->table[i].begin(), this->table[i].end(), true);
        }
    }

    int findValidConfigs(const std::vector<std::string>& signals)
    {
        std::array<Segment, SEGMENT_COUNT> chosen{};
        int count{};

        findValidConfigs(Segment::A, signals, chosen, count);

        if (count == 1)
        {
            for (Segment seg{Segment::A}; seg < Segment::COUNT; ++seg)
            {
                removeAllExcept(seg, chosen[static_cast<std::size_t>(seg)]);
            }
        }

        return count;
    }

    constexpr void removeOption(const Segment input, const Segment output) noexcept
    {
        const std::size_t inputIndex{static_cast<std::size_t>(input)};
        const std::size_t outputIndex{static_cast<std::size_t>(output)};

        this->table[inputIndex][outputIndex] = false;
    }

    template <typename... Args>
    constexpr void removeAllExcept(const Segment input, Args&&... args) noexcept
    {
        const std::size_t inputIndex{static_cast<std::size_t>(input)};

        const std::array<Segment, sizeof...(args)> outputs{args...};
        std::array<bool, sizeof...(args)> alreadyRemoved{};

        for (std::size_t i{}; i < outputs.size(); ++i)
        {
            const std::size_t outputIndex{static_cast<std::size_t>(outputs[i])};

            if (!this->table[inputIndex][outputIndex])
                alreadyRemoved[i] = true;
        }

        this->removeAllOptions(input);

        for (std::size_t i{}; i < outputs.size(); ++i)
        {
            const std::size_t outputIndex{static_cast<std::size_t>(outputs[i])};

            if (!alreadyRemoved[i])
            {
                this->table[inputIndex][outputIndex] = true;
            }
        }
    }

    constexpr void removeAllOptions(const Segment input) noexcept
    {
        const std::size_t inputIndex{static_cast<std::size_t>(input)};

        std::fill(this->table[inputIndex].begin(), this->table[inputIndex].end(), false);
    }

    [[nodiscard]]
    constexpr Segment convert(const Segment input) const noexcept
    {
        const std::size_t index{static_cast<std::size_t>(input)};

        assert(std::count(this->table[index].begin(), this->table[index].end(), true) == 1);

        return static_cast<Segment>(std::distance(this->table[index].begin(), std::find(this->table[index].begin(), this->table[index].end(), true)));
    }
private:
    std::array<std::array<bool, SEGMENT_COUNT>, SEGMENT_COUNT> table;

    bool solutionFound{};

    void findValidConfigs(const Segment start, const std::vector<std::string>& signals, std::array<Segment, SEGMENT_COUNT>& chosen, int& count)
    {
        if (start == Segment::COUNT)
        {
            if (containsDuplicateSegments(chosen))
                return;

            bool allValid{true};

            for (const std::string& signal : signals)
            {
                // H for debugging
                std::string converted(signal.size(), 'H');

                for (std::size_t i{}; i < signal.size(); ++i)
                {
                    const std::size_t index{static_cast<std::size_t>(signal[i] - 'a')};
                    converted[i] = segmentToChar(chosen[index]);
                }

                std::sort(converted.begin(), converted.end());

                if (!isValidSignal(converted))
                {
                    allValid = false;
                    break;
                }
            }

            if (allValid)
            {   
                ++count;

                this->solutionFound = true;
            }
        }
        else
        {
            const std::size_t segIndex{static_cast<std::size_t>(start)};

            for (std::size_t i{}; i < this->table[segIndex].size(); ++i)
            {
                // if (this->table[segIndex][i] && std::find(chosen.begin(), chosen.begin() + segIndex + 1, static_cast<Segment>(i)) == chosen.begin() + segIndex + 1)
                if (this->table[segIndex][i])
                {
                    chosen[segIndex] = static_cast<Segment>(i);
                    findValidConfigs(start + 1, signals, chosen, count);

                    if (this->solutionFound)
                        return;
                }
            }
        }
    }
};

int main()
{
    std::ifstream infile{"input.txt"};

    int answer{};

    std::string line;
    while (std::getline(infile, line))
    {
        std::vector<std::string> signals;

        std::stringstream ss{line};

        SignalConverter converter;

        while (true)
        {
            std::string signal;
            ss >> signal;

            if (signal == "|")
                break;
            
            switch (signal.size())
            {
                // Number 1
                case 2:
                {
                    const Segment seg1{charToSegment(signal[0])};
                    const Segment seg2{charToSegment(signal[1])};

                    converter.removeAllExcept(seg1, Segment::C, Segment::F);
                    converter.removeAllExcept(seg2, Segment::C, Segment::F);

                    for (Segment seg{Segment::A}; seg < Segment::COUNT; ++seg)
                    {
                        if (seg != seg1 && seg != seg2)
                        {
                            converter.removeOption(seg, Segment::C);
                            converter.removeOption(seg, Segment::F);
                        }
                    }

                    break;
                }
                // Number 4
                case 4:
                {
                    const Segment seg1{charToSegment(signal[0])};
                    const Segment seg2{charToSegment(signal[1])};
                    const Segment seg3{charToSegment(signal[2])};
                    const Segment seg4{charToSegment(signal[3])};

                    converter.removeAllExcept(seg1, Segment::B, Segment::C, Segment::D, Segment::F);
                    converter.removeAllExcept(seg2, Segment::B, Segment::C, Segment::D, Segment::F);
                    converter.removeAllExcept(seg3, Segment::B, Segment::C, Segment::D, Segment::F);
                    converter.removeAllExcept(seg4, Segment::B, Segment::C, Segment::D, Segment::F);

                    for (Segment seg{Segment::A}; seg < Segment::COUNT; ++seg)
                    {
                        if (seg != seg1 && seg != seg2 && seg != seg3 && seg != seg4)
                        {
                            converter.removeOption(seg, Segment::B);
                            converter.removeOption(seg, Segment::C);
                            converter.removeOption(seg, Segment::D);
                            converter.removeOption(seg, Segment::F);
                        }
                    }

                    break;
                }
                // Number 7
                case 3:
                {
                    const Segment seg1{charToSegment(signal[0])};
                    const Segment seg2{charToSegment(signal[1])};
                    const Segment seg3{charToSegment(signal[2])};

                    converter.removeAllExcept(seg1, Segment::A, Segment::C, Segment::F);
                    converter.removeAllExcept(seg2, Segment::A, Segment::C, Segment::F);
                    converter.removeAllExcept(seg3, Segment::A, Segment::C, Segment::F);

                    for (Segment seg{Segment::A}; seg < Segment::COUNT; ++seg)
                    {
                        if (seg != seg1 && seg != seg2 && seg != seg3)
                        {
                            converter.removeOption(seg, Segment::A);
                            converter.removeOption(seg, Segment::C);
                            converter.removeOption(seg, Segment::F);
                        }
                    }

                    break;
                }
            }

            signals.push_back(std::move(signal));
        }

        const int validConfigCount{converter.findValidConfigs(signals)};
        assert(validConfigCount == 1);

        int displayNumber{};

        for (int i{3}; i > -1; --i)
        {
            std::string output;
            ss >> output;

            for (std::size_t j{}; j < output.size(); ++j)
            {
                output[j] = segmentToChar(converter.convert(charToSegment(output[j])));
            }

            std::sort(output.begin(), output.end());

            displayNumber += displayToInt(output) * static_cast<int>(std::pow(10, i));
        }

        answer += displayNumber;
    }

    std::cout << "Answer: " << answer << '\n';

    return 0;
}