#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"

/**
 * @brief Insert the number (if present) containing the tile at x, y in grid to partNumbers
 * 
 * @param grid 
 * @param gridHeight 
 * @param gridWidth 
 * @param partNumbers 
 * @param x 
 * @param y 
 */
static void processNeighbor(char** const grid, const size_t gridHeight, const size_t gridWidth, BSTMap* const partNumbers, const size_t x, const size_t y)
{
    // No needed to check less than zero because std::size_t is unsigned
    if (y >= gridHeight || x >= gridWidth)
        return;

    if (isdigit(grid[y][x]))
    {
        // Find start of number
        // Heap allocate the pointer for storage in the BST
        char** first = malloc(sizeof(char*));
        *first = grid[y] + x;
        while (*first != grid[y] && isdigit(*((*first) - 1)))
        {
            --(*first);
        }

        // Find last of number
        // No need for heap allocation becaues end is never stored in BST
        char* last = grid[y] + x;
        while (last != grid[y] + gridWidth - 1 && isdigit(*(last + 1)))
        {
            ++last;
        }

        // Length of part number string not counting null terminator
        const size_t partNumberStrSize = (size_t)(last - *first + 1);

        // Copy string into buffer and add null terminator
        char* const partNumberStr = malloc(sizeof(char) * (partNumberStrSize + 1));
        memcpy(partNumberStr, *first, partNumberStrSize);
        partNumberStr[partNumberStrSize] = '\0';

        // Convert part number string to int. Again stored on heap for BST ownership
        int* const partNumber = malloc(sizeof(int));
        *partNumber = atoi(partNumberStr);

        // Free part number string because only the numeric part number is used later
        free(partNumberStr);

        // first and partNumber will be freed if they are not inserted (e.i. if first is a duplicate)
        bstMapInsert(partNumbers, first, partNumber);
    }
}

/**
 * @brief Perform strcmp-like comparisions of the char pointers pointed to by the two void pointers
 * 
 * @param lhsVoid pointer to char pointer
 * @param rhsVoid  pointer to char pointer
 * @return three-way comparison of the pointers pointed to by lhsVoid and rhsVoid
 */
static int compareKeys(const void* const lhsVoid, const void* const rhsVoid)
{
    const char* const * const lhs = lhsVoid;
    const char* const * const rhs = rhsVoid;

    if (*lhs == *rhs)
        return 0;
    else if (*lhs < *rhs)
        return -1;
    else
        return 1;
}

/**
 * @brief Multiply the int pointed to by value to the int pointed to by sum
 * 
 * @param key 
 * @param value 
 * @param sum 
 */
static void multiplyEntries(void* const key, void* const value, void* const sum)
{
    (void)key;
    *((int*)sum) *= *((int*)value);
}

int main(const int argc, const char** const argv)
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <INPUT_FILE>\n", argv[0]);
        return 1;
    }

    FILE* const infile = fopen(argv[1], "r");
    if (infile == NULL)
    {
        perror("Could not open input file");
        return 1;
    }

    // Count lines for memory allocation later
    const size_t lineCount = countLines(infile);
    if (lineCount == (size_t)-1)
    {
        fputs("Failed to count lines of input file\n", stderr);
        goto cleanup2;
    }

    char** const grid = malloc(sizeof(char*) * lineCount);
    size_t gridHeight = 0;
    size_t gridWidth = 0;

    LineBuffer line;
    lineBufferCreate(&line);

    // Read file line by line and store them in grid
    while (readNextLine(infile, &line))
    {
        if (gridWidth == 0)
            gridWidth = line.size;
        
        // All lines must be the same length
        if (line.size != gridWidth)
        {
            fputs("ERROR! Inconsistant line lengths", stderr);
            goto cleanup1;
        }

        grid[gridHeight] = malloc(sizeof(char) * gridWidth);
        memcpy(grid[gridHeight], line.line, line.size);

        ++gridHeight;
    }

    // Puzzle result
    int ratioSum = 0;

    // Iterate over each tile in grid
    for (size_t y = 0; y < gridHeight; ++y)
    {
        for (size_t x = 0; x < gridWidth; ++x)
        {
            const char c = grid[y][x];

            // Process all neighbors if c is a '*'
            if (c == '*')
            {
                BSTMap partNumbers;
                bstMapCreate(&partNumbers, compareKeys);

                processNeighbor(grid, gridWidth, gridHeight, &partNumbers, x, y - 1); // North
                processNeighbor(grid, gridWidth, gridHeight, &partNumbers, x, y + 1); // South
                processNeighbor(grid, gridWidth, gridHeight, &partNumbers, x + 1, y); // East
                processNeighbor(grid, gridWidth, gridHeight, &partNumbers, x - 1, y); // West
                processNeighbor(grid, gridWidth, gridHeight, &partNumbers, x + 1, y - 1); // North-East
                processNeighbor(grid, gridWidth, gridHeight, &partNumbers, x + 1, y + 1); // South-East
                processNeighbor(grid, gridWidth, gridHeight, &partNumbers, x - 1, y + 1); // South-West
                processNeighbor(grid, gridWidth, gridHeight, &partNumbers, x - 1, y - 1); // North-West

                // c is only a gear if it has exactly 2 neighboring part numbers
                if (bstMapSize(&partNumbers) == 2)
                {
                    int ratio = 1;
                    bstMapForEach(&partNumbers, multiplyEntries, &ratio);
                    ratioSum += ratio;
                }

                bstMapDestroy(&partNumbers);
            }
        }
    }

    printf("Result: %d\n", ratioSum);

cleanup1:
    lineBufferDestroy(&line);

cleanup2:
    for (size_t i = 0; i < gridHeight; ++i)
    {
        free(grid[i]);
    }
    free(grid);

    if (fclose(infile) == EOF)
    {
        fputs("fclose() failed\n", stderr);
        return 1;
    }

    return 0;
}