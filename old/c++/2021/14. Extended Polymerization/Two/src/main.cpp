#include <algorithm>
#include <array>
#include <cstddef>
#include <fstream>
#include <functional>
#include <iostream>
#include <iterator>
#include <limits>
#include <numeric>
#include <string>
#include <vector>

/**
 * @brief Insertion rule to insert 'between' between 'left' and 'right'
 * 
 */
struct InsertionRule
{
    char left{};
    char right{};
    char between{};
};

class Polymer
{
public:
    char last{};

    /**
     * @brief Return a reference to the number of times left has occured before right
     * 
     * @param left Left char of pair
     * @param right Right char of pair
     * @return Reference to the occurence count
     */
    [[nodiscard]]
    constexpr std::size_t& lookup(const char left, const char right)
    {
        return this->matrix[static_cast<std::size_t>(left) - 'A'][static_cast<std::size_t>(right) - 'A'];
    }

    /**
     * @brief Preform one insertion step with the given rules
     * 
     * @param rules Rules for insertions
     */
    void insertionStep(const std::vector<InsertionRule>& rules)
    {
        Polymer result{*this};

        for (const auto& rule : rules)
        {
            const std::size_t ammount{lookup(rule.left, rule.right)};
            result.lookup(rule.left, rule.between) += ammount;
            result.lookup(rule.between, rule.right) += ammount;
            result.lookup(rule.left, rule.right) -= ammount;
        }

        *this = result;
    }

    /**
     * @brief Count how many times each character occurs
     * 
     * @return Array of counts where the index is a letter (1 through 25) and the value is the count of occurences
     */
    [[nodiscard]]
    constexpr std::array<std::size_t, 26> counts() const
    {
        std::array<std::size_t, 26> result{};

        for (std::size_t i{}; i < result.size(); ++i)
        {
            result[i] = std::accumulate(this->matrix[i].begin(), this->matrix[i].end(), static_cast<std::size_t>(0));
        }

        ++result[static_cast<std::size_t>(this->last) - 'A'];

        return result;
    }

    /**
     * @brief Return the number of characters
     * 
     * @return Number of characters
     */
    [[nodiscard]]
    constexpr std::size_t size() const
    {
        const std::array<std::size_t, 26> countValues{counts()};
        return std::accumulate(countValues.begin(), countValues.end(), static_cast<std::size_t>(0));
    }
private:
    std::array<std::array<std::size_t, 26>, 26> matrix{};
};

/**
 * @brief Read starting polymer from first line of file
 * 
 * @param infile File to read from
 * @return Starting polymer value
 */
static Polymer readStartingPolymer(std::ifstream& infile)
{
    Polymer polymer;

    std::string line;
    std::getline(infile, line);

    polymer.last = line.back();

    for (std::size_t i{}; i < line.size() - 1; ++i)
    {
        ++polymer.lookup(line[i], line[i + 1]);
    }

    return polymer;
}

/**
 * @brief Read rules from file
 * 
 * @param infile File to read from
 * @return Insertion rules
 */
static std::vector<InsertionRule> readRules(std::ifstream& infile)
{
    std::vector<InsertionRule> rules;

    char left;
    char right;
    char between;

    while (infile >> left >> right)
    {
        // Ignore arrow
        infile.ignore(4);

        infile >> between;

        rules.emplace_back(left, right, between);
    }

    return rules;
}

int main()
{
    static constexpr int STEP_COUNT{40};

    std::ifstream infile{"input.txt"};

    if (!infile.is_open())
    {
        std::cerr << "ERROR! Could not open infile!\n";
        return 1;
    }

    Polymer polymer{readStartingPolymer(infile)};

    // Ignore emtpy line
    infile.ignore();

    std::vector<InsertionRule> rules{readRules(infile)};

    for (int i{}; i < STEP_COUNT; ++i)
    {
        polymer.insertionStep(rules);
    }

    std::array<std::size_t, 26> counts{polymer.counts()};

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