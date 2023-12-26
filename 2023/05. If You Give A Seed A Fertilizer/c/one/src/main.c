#include <ctype.h>
#include <stdio.h>
#include <string.h>

#include "utils.h"

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
    dynamicArrayCreate(&seeds, sizeof(unsigned long long int));

    // Read seeds
    for (unsigned long long int seed; fscanf(infile, "%llu", &seed) == 1;)
    {
        dynamicArrayPushBack(&seeds, &seed, 1);
    }

    // Array of bool flags representing if a seed at a given index was covnerted during the current map
    // Reset at start of each map
    DynamicArray seedConverted;
    dynamicArrayCreate(&seedConverted, sizeof(unsigned char));
    dynamicArrayResize(&seedConverted, seeds.size);

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
            unsigned long long int destStart;
            unsigned long long int srcStart;
            unsigned long long int size;
            if (sscanf(line.line, "%llu %llu %llu", &destStart, &srcStart, &size) != 3)
            {
                fprintf(stderr, "ERROR! Failed to read line %d\n", lineNum);
                exitStatus = 1;
                goto cleanup;
            }

            // Try to convert all seeds
            for (size_t i = 0; i < seeds.size; ++i)
            {
                unsigned long long int* const seed = (unsigned long long*)dynamicArrayIndex(&seeds, i);
                unsigned char* const converted = (unsigned char*)dynamicArrayIndex(&seedConverted, i);
                if (!*converted && *seed >= srcStart && *seed < srcStart + size)
                {
                    *converted = true;
                    *seed = destStart + *seed - srcStart;
                }
            }
        }
        // Otherwise a new mapping is starting. Reset conversion flags
        else
        {
            memset(seedConverted.array, 0, sizeof(unsigned char) * seedConverted.size);
        }
    }

    printf("Result: %llu\n", *minull((const unsigned long long int*)seeds.array, seeds.size));

cleanup:
    lineBufferDestroy(&line);

    dynamicArrayDestroy(&seedConverted);
    dynamicArrayDestroy(&seeds);

    fclose(infile);

    return exitStatus;
}