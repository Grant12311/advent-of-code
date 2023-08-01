#include <algorithm>
#include <cstddef>
#include <concepts>
#include <fstream>
#include <functional>
#include <iostream>
#include <limits>
#include <unordered_map>
#include <utility>
#include <vector>

/**
 * @brief Binary Heap
 * 
 * @tparam T Value Type
 * @tparam Comp Binary Predicate comparison function implementing less than
 */
template <typename T, std::predicate<T, T> Comp = std::less<T>>
class Heap
{
public:
    Heap(const Comp& a_comp) :
        comp{a_comp}
    {}

    /**
     * @brief Insert object of type U compatible with type T
     * 
     * @tparam U 
     * @param value 
     */
    template <typename U>
    void insert(U&& value)
    {
        this->array.push_back(std::forward<U>(value));
        this->indices.insert({this->array.back(), this->array.size() - 1});
        heapifyUp();
    }

    /**
     * @brief Return top (min) element from heap
     * 
     * @return Top (min) element from heap
     */
    [[nodiscard]]
    T& top()
    {
        return this->array.front();
    }

    /**
     * @brief Return top (min) element from heap
     * 
     * @return Top (min) element from heap
     */
    [[nodiscard]]
    const T& top() const
    {
        return this->array.front();
    }

    /**
     * @brief Remove top element from heap
     * 
     */
    void pop()
    {
        this->indices.erase(this->array.front());
        std::swap(this->array.front(), this->array.back());
        this->array.pop_back();

        heapifyDown();
    }

    /**
     * @brief Reevaluate the placement of the given value
     * 
     * @param value 
     */
    void decrease(const T& value)
    {
        heapifyUp(this->indices.at(value));
    }
private:
    /**
     * @brief Calculate left child index of node
     * 
     * @param node Parent index
     * @return Left child index of node
     */
    [[nodiscard]]
    static constexpr std::size_t leftIndex(const std::size_t node) noexcept
    {
        return (node * 2) + 1;
    }

    /**
     * @brief Calculate right child index of node
     * 
     * @param node Parent index
     * @return Right child index of node
     */
    [[nodiscard]]
    static constexpr std::size_t rightIndex(const std::size_t node) noexcept
    {
        return (node * 2) + 2;
    }

    /**
     * @brief Calculate parent index of node
     * 
     * @param node Child index
     * @return Parent index of node
     */
    [[nodiscard]]
    static constexpr std::size_t parentIndex(const std::size_t node) noexcept
    {
        return (node - 1) / 2;
    }

    /**
     * @brief Correct a heap upwards from the bottom
     * 
     */
    void heapifyUp()
    {
        heapifyUp(this->array.size() - 1);
    }

    /**
     * @brief Correct a heap upwards from the given index
     * 
     * @param node Index to work updwards from
     */
    void heapifyUp(const std::size_t node)
    {
        if (node == 0)
            return;

        const std::size_t parentPos{parentIndex(node)};

        T& self{this->array[node]};
        T& parent{this->array[parentPos]};

        if (this->comp(self, parent))
        {
            this->indices.at(self) = parentPos;
            this->indices.at(parent) = node;
            std::swap(self, parent);

            heapifyUp(parentPos);
        }
    }

    /**
     * @brief Correct heap downwards from given index
     * 
     * @param node Index to start from
     */
    void heapifyDown(const std::size_t node = 0)
    {
        const std::size_t leftPos{leftIndex(node)};

        if (leftPos >= this->array.size())
            return;

        T* const self{this->array.data() + node};
        T* const left{this->array.data() + leftPos};
        T* minChild = left;

        const std::size_t rightPos{rightIndex(node)};
        T* const right{this->array.data() + rightPos};

        if (rightPos < this->array.size())
            minChild = const_cast<T*>(&std::min(*left, *right, this->comp));

        if (this->comp(*minChild, *self))
        {
            const std::size_t minChildPos{static_cast<std::size_t>(std::distance(this->array.data(), minChild))};

            this->indices.at(*self) = minChildPos;
            this->indices.at(*minChild) = node;
            std::swap(*self, *minChild);

            heapifyDown(minChildPos);
        }
    }

    std::vector<T> array;
    std::unordered_map<T, std::size_t> indices;
    Comp comp{};
};

/**
 * @brief Read map from file
 * 
 * @param infile File to read from
 * @return Map as 2D vector
 */
[[nodiscard]]
static std::vector<std::vector<int>> readMap(std::ifstream& infile)
{
    std::vector<std::vector<int>> map(1);

    char value;
    while (infile.get(value))
    {
        if (value == '\n')
            map.emplace_back();
        else
            map.back().push_back(value - '0');
    }

    map.pop_back();

    return map;
}

struct Point
{
    std::size_t x{};
    std::size_t y{};

    constexpr bool operator==(const Point& rhs) const = default;
};

template <>
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

/**
 * @brief Run Dijkstra's shortest path to find shortest path from upper right corner to upper left corner
 * 
 * @param map
 * @return Shortest path from upper right corner to upper left corner
 */
static int dijkstras(const std::vector<std::vector<int>>& map)
{
    std::vector<std::vector<int>> distances(map.size());
    std::ranges::fill(distances, std::vector<int>(map.front().size(), std::numeric_limits<int>::max()));
    distances[0][0] = 0;

    // Compare two points based on their distance from the above array
    const auto compPoints{[&distances](const Point& lhs, const Point& rhs) -> bool
    {
        return distances[lhs.y][lhs.x] < distances[rhs.y][rhs.x];
    }};

    // Priority queue for node with shortest distance
    Heap<Point, decltype(compPoints)> queue{compPoints};

    for (std::size_t y{}; y < map.size(); ++y)
    {
        for (std::size_t x{}; x < map[y].size(); ++x)
        {
            queue.insert(Point{x, y});
        }
    }

    // Run until the dest node is the closest
    while (queue.top() != Point{map.front().size() - 1, map.size() - 1})
    {
        const Point& point{queue.top()};
        const int pointDistance{distances[point.y][point.x]};

        // Check what directions have neighbors
        const bool goNorth{point.y > 0};
        const bool goSouth{point.y < map.size() - 1};
        const bool goEast{point.x < map.front().size() - 1};
        const bool goWest{point.x > 0};

        // Array of neighbors that exist
        std::array<Point, 4> explore;
        std::size_t exploreCount{};

        if (goNorth)
            explore[exploreCount++] = {point.x, point.y - 1};
        if (goSouth)
            explore[exploreCount++] = {point.x, point.y + 1};
        if (goEast)
            explore[exploreCount++] = {point.x + 1, point.y};
        if (goWest)
            explore[exploreCount++] = {point.x - 1, point.y};

        // Check if any neighbors should be updated to travel via the current node
        for (std::size_t i{}; i < exploreCount; ++i)
        {
            const Point& neighbor{explore[i]};
            int& neighborDistance{distances[neighbor.y][neighbor.x]};

            const int candidateDistance{pointDistance + map[neighbor.y][neighbor.x]};

            // If the neighbor is unvisted or the current node is better as a via node, update the path
            if (neighborDistance == std::numeric_limits<int>::max() || candidateDistance < neighborDistance)
            {
                neighborDistance = candidateDistance;
                queue.decrease(neighbor);
            }
        }

        // Move on to the next node
        queue.pop();
    }

    return distances.back().back();
}

int main()
{
    std::ifstream infile{"input.txt"};

    if (!infile.is_open())
    {
        std::cerr << "ERROR! Could not open infile!\n";
        return 1;
    }

    const std::vector<std::vector<int>> map{readMap(infile)};

    std::cout << "Result: " << dijkstras(map) << '\n';

    return 0;
}