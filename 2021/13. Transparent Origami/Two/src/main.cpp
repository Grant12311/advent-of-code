#include <algorithm>
#include <cstddef>
#include <fstream>
#include <functional>
#include <iostream>
#include <string_view>
#include <set>

struct Point
{
    std::size_t x{};
    std::size_t y{};

    constexpr bool operator<(const Point& rhs) const noexcept
    {
        if (this->y == rhs.y)
            return this->x < rhs.x;
        else
            return this->y < rhs.y;
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
static std::set<Point> foldX(const std::set<Point>& points, const std::size_t line)
{
    std::set<Point> result;

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
static std::set<Point> foldY(const std::set<Point>& points, const std::size_t line)
{
    std::set<Point> result;

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
static std::set<Point> fold(const std::set<Point>& points, const std::size_t line, const Along along)
{
    if (along == Along::X)
        return foldX(points, line);
    else
        return foldY(points, line);
}

/**
 * @brief Print paper to stdout
 * 
 * @param points Paper to print
 */
static void render(const std::set<Point>& points)
{
    // Find X largest X coord from points to use as width
    const std::size_t width{std::ranges::max_element(points, {}, &Point::x)->x + 1};

    // Position to contiune from
    Point last;

    for (const Point& point : points)
    {
        // If we need to move down, finish the current line and print empty lines we need to skip
        for (; last.y < point.y; ++last.y)
        {
            for (; last.x < width; ++last.x)
            {
                std::cout << '.';
            }
            std::cout << '\n';

            // Reset to start of line after finishing a line
            last.x = 0;
        }

        // Print portion of line before point
        for (; last.x < point.x; ++last.x)
        {
            std::cout << '.';
        }

        // Print point
        std::cout << '#';
        ++last.x;
    }
    std::cout << '\n';
}

int main()
{
    std::ifstream infile{"input.txt"};

    if (!infile.is_open())
    {
        std::cerr << "ERROR! Could not open input file!\n";
        return 1;
    }

    // Store points currently on the paper as a set. Ordered for rendering later
    std::set<Point> points;

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
    }

    std::cout << "Result:\n";
    render(points);

    return 0;
}