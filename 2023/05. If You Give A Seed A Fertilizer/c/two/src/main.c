#include <ctype.h>
#include <stdio.h>
#include <string.h>

#include "utils.h"

typedef struct
{
    long long int start;
    long long int size;
} SeedRange;

/**
 * @brief Return true if the SeedRange pointed to by lhsVoid has a start less than rhsVoid's SeedRange
 * 
 * @param lhsVoid 
 * @param rhsVoid 
 * @return true 
 * @return false 
 */
static bool seedRangeLessThan(const void* const lhsVoid, const void* const rhsVoid)
{
    const SeedRange* const lhs = lhsVoid;
    const SeedRange* const rhs = rhsVoid;

    return lhs->start < rhs->start;
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
        fprintf(stderr, "ERROR! Failed to open \"%s\"\n", argv[1]);
        return 1;
    }

    // Verify start of seed line
    int bytesRead = 0;
    fscanf(infile, "seeds: %n", &bytesRead);
    if (bytesRead != 7)
    {
        fputs("ERROR! Failed to read seeds list\n", stderr);
        fclose(infile);
        return 1;
    }

    // Used to make cleanup easier
    int exitStatus = 0;

    DynamicArray seeds;
    dynamicArrayCreate(&seeds, sizeof(SeedRange));

    // Read seed ranges
    for (long long int start, size; fscanf(infile, "%lld %lld", &start, &size) == 2;)
    {
        const SeedRange range = {.start = start, .size = size};
        dynamicArrayPushBack(&seeds, &range, 1);
    }

    // Array of bool flags representing if a seed range at a given index was covnerted during the current map
    // Reset at start of each map
    DynamicArray rangeConverted;
    dynamicArrayCreate(&rangeConverted, sizeof(unsigned char));
    dynamicArrayResize(&rangeConverted, seeds.size);

    // New breakaway ranges to insert into seeds after each seed is converted. Cleared after insertion
    DynamicArray newSeeds;
    dynamicArrayCreate(&newSeeds, sizeof(SeedRange));

    // Read each line

    LineBuffer line;
    lineBufferCreate(&line);

    for (int lineNum = 1; readNextLine(infile, &line); ++lineNum)
    {
        // Skip empty lines
        if (line.size == 0)
            continue;

        // Process mapping if line starts with digit
        if (isdigit(line.line[0]))
        {
            // Read mapping
            long long int destStart;
            long long int srcStart;
            long long int size;
            if (sscanf(line.line, "%lld %lld %lld", &destStart, &srcStart, &size) != 3)
            {
                fprintf(stderr, "ERROR! Failed to read line %d\n", lineNum);
                exitStatus = 1;
                goto cleanup;
            }

            // Try to convert all seeds
            for (size_t i = 0; i < seeds.size; ++i)
            {
                unsigned char* const converted = (unsigned char*)dynamicArrayIndex(&rangeConverted, i);
                if (*converted)
                    continue;

                SeedRange* const startingRange = (SeedRange*)dynamicArrayIndex(&seeds, i);

                // If any part of this range should be converted
                if (startingRange->start + startingRange->size - 1 >= srcStart && startingRange->start < srcStart + size)
                {
                    // Break off portion before start of conversion area
                    if (startingRange->start < srcStart)
                    {
                        const SeedRange newRange = {.start = startingRange->start, srcStart - startingRange->start};
                        dynamicArrayPushBack(&newSeeds, &newRange, 1);

                        startingRange->size -= srcStart - startingRange->start;
                        startingRange->start = srcStart;
                    }

                    // Break off portion before after end of conversion area
                    if (startingRange->start + startingRange->size > srcStart + size)
                    {
                        const SeedRange newRange = {.start = srcStart + size, startingRange->start + startingRange->size - (srcStart + size)};
                        dynamicArrayPushBack(&newSeeds, &newRange, 1);

                        startingRange->size -= startingRange->start + startingRange->size - (srcStart + size);
                    }

                    // Convert remaining portion
                    startingRange->start = destStart + startingRange->start - srcStart;
                    *converted = 1;

                    // Insert any new seed ranges from above breakaway code
                    dynamicArrayPushBack(&seeds, newSeeds.array, newSeeds.size);
                    dynamicArrayClear(&newSeeds);

                    // Resize to match current seed count
                    dynamicArrayResize(&rangeConverted, seeds.size);
                }
            }
        }
        // Otherwise a new mapping is starting. Reset conversion flags
        else
        {
            memset(rangeConverted.array, 0, sizeof(unsigned char) * rangeConverted.size);
        }
    }

    printf("Result: %lld\n", ((const SeedRange*)min(seeds.array, sizeof(SeedRange), seeds.size, seedRangeLessThan))->start);

cleanup:
    lineBufferDestroy(&line);

    dynamicArrayDestroy(&newSeeds);
    dynamicArrayDestroy(&rangeConverted);
    dynamicArrayDestroy(&seeds);

    fclose(infile);

    return exitStatus;
}