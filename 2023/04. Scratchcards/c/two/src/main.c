#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "utils.h"

/**
 * @brief Compare the ints pointed to by two void pointers with strcmp-like three-way comparison
 * 
 * @param lhsVoid 
 * @param rhsVoid 
 * @return int 
 */
static int compareInts(const void* const lhsVoid, const void* const rhsVoid)
{
    const int lhs = *(int*)lhsVoid;
    const int rhs = *(int*)rhsVoid;

    return lhs - rhs;
}

int main(const int argc, const char** const argv)
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <INPUT_FILE>\n", argv[0]);
    }

    // Puzzle result
    int cardCount = 0;

    FILE* const infile = fopen(argv[1], "r");
    if (infile == NULL)
    {
        fputs("ERROR! Failed to open input file\n", stderr);
        return 1;
    }

    // Both number sets are cleared after every iteration

    // Numbers present in card
    DynamicArray presentNumbers;
    dynamicArrayCreate(&presentNumbers, sizeof(int));

    // Winning numbers from card
    DynamicArray winningNumbers;
    dynamicArrayCreate(&winningNumbers, sizeof(int));

    // Number of extra copies to add for the next card is present in front
    DynamicArray copies;
    dynamicArrayCreate(&copies, sizeof(int));

    // Read each line

    LineBuffer line;
    lineBufferCreate(&line);

    for (size_t lineNum = 0; readNextLine(infile, &line); ++lineNum, dynamicArrayClear(&presentNumbers), dynamicArrayClear(&winningNumbers))
    {
        // Total bytes read of line. Updated after every read
        ptrdiff_t bytesRead = 0;

        // Read and verify line opening
        size_t cardNum = 0;
        if (sscanf(line.line, "Card %zu:%tn", &cardNum, &bytesRead) != 1)
        {
            fprintf(stderr, "ERROR! Line %zu is invalid\n", lineNum);
            goto cleanup;
        }

        // Used to update bytesRead after each read
        ptrdiff_t newBytesRead = 0;

        // Read present numbers. Stops when pipe is hit
        for (int number; sscanf(line.line + bytesRead, "%d%tn", &number, &newBytesRead) == 1; bytesRead += newBytesRead)
        {
            dynamicArrayPushBack(&presentNumbers, &number, 1);
        }

        // Read and verify pipe
        char c = '\0';
        if (sscanf(line.line + bytesRead, " %c %tn", &c, &newBytesRead) != 1)
        {
            fprintf(stderr, "ERROR! Line %zu is invalid\n", lineNum);
            goto cleanup;
        }
        bytesRead += newBytesRead;

        // Read winning numbers. Stops when line ends
        for (int number; sscanf(line.line + bytesRead, "%d%tn", &number, &newBytesRead) == 1; bytesRead += newBytesRead)
        {
            dynamicArrayPushBack(&winningNumbers, &number, 1);
        }

        // Sort winning numbers for binary search
        qsort(winningNumbers.array, winningNumbers.size, winningNumbers.elementSize, compareInts);

        // Calculate card score
        size_t wins = 0;
        for (size_t i = 0; i < presentNumbers.size; ++i)
        {
            if (bsearch(dynamicArrayIndex(&presentNumbers, i), winningNumbers.array, winningNumbers.size, winningNumbers.elementSize, compareInts) != NULL)
                ++wins;
        }

        // Add extra copies of necessary
        int copiesCurrent = 1;
        if (copies.size > 0)
        {
            copiesCurrent += *(const int*)dynamicArrayIndex(&copies, 0);
            dynamicArrayErase(&copies, 0);
        }

        // Add copies to total card count
        cardCount += copiesCurrent;

        // Resize copies buffer to ensure enough room them increment copy counts
        dynamicArrayResize(&copies, wins > copies.size ? wins : copies.size);
        for (size_t i = 0; i < wins; ++i)
        {
            *(int*)dynamicArrayIndex(&copies, i) += copiesCurrent;
        }
    }

    printf("Result: %d\n", cardCount);

cleanup:
    if (fclose(infile) == EOF)
        fputs("ERROR! Failed to close input file\n", stderr);

    dynamicArrayDestroy(&copies);
    dynamicArrayDestroy(&presentNumbers);
    dynamicArrayDestroy(&winningNumbers);

    lineBufferDestroy(&line);
}