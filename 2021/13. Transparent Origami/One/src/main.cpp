#include <cstddef>
#include <fstream>
#include <functional>
#include <iostream>
#include <string_view>
#include <unordered_set>

struct Point
{
    std::size_t x{};
    std::size_t y{};

    constexpr bool operator==(const Point& rhs) const = default;
};

template<>
struct std::hash<Point>
{
    std::size_t operator()(const Point& point) const noexcept
    {
        const std::hash<std::size_t> func;
        const std::size_t xHash{func(point.x)};
        const std::size_t yHash{func(point.y)};
        return xHash ^ (yHash << 1);
    }
};

enum class Along
{
    X,
    Y
};

// Text that starts each fold line
static constexpr std::string_view FOLD_PROMPT{"fold along "};

/**
 * @brief Fold along the X axis
 * 
 * @param points Paper to fold
 * @param line Point to fold along
 * @return Folded version of the paper
 */
static std::unordered_set<Point> foldX(const std::unordered_set<Point>& points, const std::size_t line)
{
    std::unordered_set<Point> result;

    for (const Point& point : points)
    {
        if (point.x < line)
            result.insert(point);
        else
            result.emplace(line - (point.x - line), point.y);
    }

    return result;
}

/**
 * @brief Fold along the Y axis
 * 
 * @param points Paper to fold
 * @param line Point to fold along
 * @return Folded version of the paper
 */
static std::unordered_set<Point> foldY(const std::unordered_set<Point>& points, const std::size_t line)
{
    std::unordered_set<Point> result;

    for (const Point& point : points)
    {
        if (point.y < line)
            result.insert(point);
        else
            result.emplace(point.x, line - (point.y - line));
    }

    return result;
}

/**
 * @brief Fold along the specified axis
 * 
 * @param points Paper to fold
 * @param line Point to fold along
 * @param along Direction to fold along
 * @return Folded version of the paper
 */
static std::unordered_set<Point> fold(const std::unordered_set<Point>& points, const std::size_t line, const Along along)
{
    if (along == Along::X)
        return foldX(points, line);
    else
        return foldY(points, line);
}

int main()
{
    std::ifstream infile{"input.txt"};

    if (!infile.is_open())
    {
        std::cerr << "ERROR! Could not open input file!\n";
        return 1;
    }

    // Store points currently on the paper as a set
    std::unordered_set<Point> points;

    std::size_t x;
    std::size_t y;

    // Read points until the breakpoint
    while (infile.peek() != '\n')
    {
        infile >> x;
        // Ignore comma
        infile.ignore();
        infile >> y;
        points.emplace(x, y);

        // Ignore newline
        infile.ignore();
    }
    // Ignore empty breakpoint line
    infile.ignore();

    // Read folds until the first character isn't the 'f' in 'fold'
    while (infile.peek() == FOLD_PROMPT.front())
    {
        // Ignore entire promt string
        infile.ignore(FOLD_PROMPT.size());

        const char axis{static_cast<char>(infile.get())};
        // Ignore equals
        infile.ignore();

        std::size_t line;
        infile >> line;
        // Ignore newline
        infile.ignore();

        // Preform fold
        points = fold(points, line, axis == 'x' ? Along::X : Along::Y);
        
        // Only one fold for part one
        break;
    }

    std::cout << "Result: " << points.size() << '\n';

    return 0;
}