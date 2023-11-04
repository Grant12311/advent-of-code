#include <cstdint>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string_view>
#include <vector>

struct Expect
{
    std::string_view value;
};

static std::istream& operator>>(std::istream& lhs, const Expect& rhs)
{
    std::vector<char> taken;
    taken.reserve(rhs.value.size());

    for (const char wanted : rhs.value)
    {
        char found;
        if (lhs.get(found).fail() || found != wanted)
        {
            for (const char c : taken)
            {
                lhs.putback(c);
            }
            lhs.putback(found);

            lhs.setstate(std::ios_base::failbit);
            break;
        }
        else
        {
            taken.push_back(found);
        }
    }

    return lhs;
}

struct Target
{
    std::uint64_t xMin{};
    std::uint64_t xMax{};
    std::uint64_t yMin{};
    std::uint64_t yMax{};
};

static std::istream& operator>>(std::istream& lhs, Target& rhs)
{
    lhs >> Expect{"target area: x="} >> rhs.xMin >> Expect{".."} >> rhs.xMax;
    lhs >> Expect{", y="} >> rhs.yMin >> Expect{".."} >> rhs.yMax;

    return lhs;
}

static std::ostream& operator<<(std::ostream& lhs, const Target& rhs)
{
    

    return lhs;
}

[[nodiscard]]
static Target loadInput()
{
    std::ifstream infile{"input2.txt"};
    if (!infile.is_open())
        throw std::runtime_error{"Cannot open input file"};

    Target target;
    infile >> target;

    return target;
}

int main()
{
    const Target target{loadInput()};

    return 0;
}