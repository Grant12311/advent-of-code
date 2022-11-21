#include <cassert>
#include <cctype>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <optional>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

/**
 * @brief Type representing a map of caves in a cave system
 * 
 */
class CaveMap
{
public:
    /**
     * @brief Add edge from n1 to n2 and from n2 to n1. Any caves not already registered will be added
     * 
     * @param n1 First node of edge
     * @param fn2 Second node of edge
     */
    void addEdge(const std::string& n1, const std::string& n2)
    {
        // Get the list of connections for n1 and n2
        // Will insert new vectors into map if they do not already exist
        std::vector<std::string>& connectionsN1{this->adj[n1]};
        std::vector<std::string>& connectionsN2{this->adj[n2]};

        // Add connection into both lists
        connectionsN1.push_back(n2);
        connectionsN2.push_back(n1);
    }

    /**
     * @brief Find the number of unique paths from start to end that only visit each small cave once 
     * 
     * @return The number of unique paths from start to end that only visit each small cave once 
     */
    int pathCount() const
    {
        // The number of paths found
        int count{};

        // Set of small caves that have already been visited
        std::unordered_set<std::string> visitedSmall{"start"};

        // Find all paths recursivly
        pathCount("start", true, count, visitedSmall);

        return count;
    }
private:
    // Adjacency list storing a graph
    std::unordered_map<std::string, std::vector<std::string>> adj;

    /**
     * @brief Check if cave name is for a small cave (all lowercase). Assert will be called to check against mixed case
     * 
     * @param name Name of cave to check
     * @return true if cave is small, else false
     */
    [[nodiscard]]
    static bool caveIsSmall(const std::string& name)
    {
        // Count of letters of each case
        int upperCount{};
        int lowerCount{};

        // Calculate the case counts
        for (const char c : name)
        {
            if (std::isupper(c))
                ++upperCount;
            else
                ++lowerCount;
        }

        // Assert that the cases are not mixed
        assert((upperCount > 0 && lowerCount == 0) || (lowerCount > 0 && upperCount == 0));

        // Cache value
        return lowerCount > 0;
    }

    /**
     * @brief Recursive function to calculate the number of unique paths through the cave system that only visit each small cave once
     * 
     * @param currentCave The cave currently being visited
     * @param canTakeExtra True if we have not already taken the extra small cave visited we're allowed
     * @param count Reference to a value storing the number of paths discovered so far
     * @param visitedSmall Set of all small caves visited so far
     */
    void pathCount(const std::string& currentCave, const bool canTakeExtra, int& count, std::unordered_set<std::string>& visitedSmall) const
    {
        // Path discovered
        if (currentCave == "end")
        {
            ++count;
        }
        else
        {
            const std::vector<std::string>& connections{this->adj.at(currentCave)};

            // Explore all caves (except visited small caves) that connect to current
            for (const std::string& cave : connections)
            {
                const bool optionIsSmall{caveIsSmall(cave)};

                if (optionIsSmall)
                {
                    // If option is small, only travel if it's either unvisited or we have not already taken our extra small cave
                    if (visitedSmall.contains(cave))
                    {
                        // If we can take an extra small cave, make sure it's not start or end first then travel, making sure to pass false
                        if (canTakeExtra && cave != "start" && cave != "end")
                            pathCount(cave, false, count, visitedSmall);
                    }
                    else
                    {
                        // Mark visited and unvisited only if option is NOT our extra small cave take
                        visitedSmall.insert(cave);
                        pathCount(cave, canTakeExtra, count, visitedSmall);
                        visitedSmall.erase(cave);
                    }
                }
                else
                {
                    // If option is large, always travel
                    pathCount(cave, canTakeExtra, count, visitedSmall);
                }
            }
        }
    }
};

int main()
{
    // Open the input file
    std::ifstream infile{"input.txt"};

    // Terminate with error if input file could not be opened
    if (!infile.is_open())
    {
        std::cerr << "ERROR! Could not open input file!\n";
        return EXIT_FAILURE;
    }

    CaveMap map;

    // Read each line of the input file
    std::string line;
    while (std::getline(infile, line))
    {
        // Split the string on each side of the dash to get the "to" and "from" points of the edge
        const std::size_t dashPos{line.find('-')};
        const std::string to{line.substr(0, dashPos)};
        const std::string from{line.substr(dashPos + 1)};

        // Add the parsed edge to the cave map
        map.addEdge(to, from);
    }

    std::cout << "Answer: " << map.pathCount() << '\n';

    return EXIT_SUCCESS;
}