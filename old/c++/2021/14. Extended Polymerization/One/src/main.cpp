#include <algorithm>
#include <array>
#include <cstddef>
#include <fstream>
#include <functional>
#include <iostream>
#include <iterator>
#include <limits>
#include <list>
#include <string>
#include <unordered_map>

struct RuleMatch
{
    char left{};
    char right{};

    constexpr bool operator==(const RuleMatch& rhs) const noexcept = default;
};

template<>
struct std::hash<RuleMatch>
{
    std::size_t operator()(const RuleMatch ruleMatch) const noexcept
    {
        std::hash<char> func;

        const std::size_t leftHash{func(ruleMatch.left)};
        const std::size_t rightHash{func(ruleMatch.right)};
        
        return leftHash ^ (rightHash << 1);
    }
};

/**
 * @brief Read starting polymer from first line of file
 * 
 * @param infile File to read from
 * @return Starting polymer value
 */
static std::list<char> readStartingPolymer(std::ifstream& infile)
{
    std::list<char> polymer;

    std::string line;
    std::getline(infile, line);

    polymer.insert(polymer.begin(), line.begin(), line.end());

    return polymer;
}

/**
 * @brief Read rules from file
 * 
 * @param infile File to read from
 * @return std::unordered_map of rules to character to insert
 */
static std::unordered_map<RuleMatch, char> readRules(std::ifstream& infile)
{
    std::unordered_map<RuleMatch, char> rules;

    char left;
    char right;
    char between;

    while (infile >> left >> right)
    {
        // Ignore arrow
        infile.ignore(4);

        infile >> between;

        rules.emplace(RuleMatch{left, right}, between);
    }

    return rules;
}

/**
 * @brief Preform one insertion step
 * 
 * @param polymer Polyer to insert into
 * @param rules Insertion rules
 */
static void insertionStep(std::list<char>& polymer, const std::unordered_map<RuleMatch, char>& rules)
{
    for (auto it{polymer.begin()}; it != std::prev(polymer.end()); ++it)
    {
        const auto rule{rules.find({*it, *std::next(it)})};

        if (rule != rules.end())
            it = polymer.insert(std::next(it), rule->second);
    }
}

int main()
{
    static constexpr int STEP_COUNT{10};

    std::ifstream infile{"input.txt"};

    if (!infile.is_open())
    {
        std::cerr << "ERROR! Could not open infile!\n";
        return 1;
    }

    std::list<char> polymer{readStartingPolymer(infile)};

    // Ignore emtpy line
    infile.ignore();

    std::unordered_map<RuleMatch, char> rules{readRules(infile)};

    for (int i{}; i < STEP_COUNT; ++i)
    {
        insertionStep(polymer, rules);
    }

    std::array<std::size_t, 128> counts{};

    for (const char c : polymer)
    {
        ++counts[static_cast<std::size_t>(c)];
    }

    const std::size_t maxValue{*std::ranges::max_element(counts)};

    // Convert zeros into size_t max so zero isn't the min
    for (std::size_t& count : counts)
    {
        if (count == 0)
            count = std::numeric_limits<std::size_t>::max();
    }

    const std::size_t minValue{*std::ranges::min_element(counts)};

    std::cout << "Result: " << maxValue - minValue << '\n';

    return 0;
}