#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"

#define DELIMS " :;,"
#define NUM_COLORS 3

static const int MAX_CUBES[NUM_COLORS] = {
    12, // red
    13, // green
    14  // blue
};

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
        perror("Failed to open input file");
        return 1;
    }

    // Puzzle result
    int idSum = 0;

    // Read each line

    LineBuffer line;
    lineBufferCreate(&line);

    for (int lineNum = 1; readNextLine(infile, &line); ++lineNum)
    {
        // Minimum number of cubes of each color needed for current game to be possilbe
        int minNeeded[NUM_COLORS] = {0};

        // Skip first two tokens
        strtok(line.line, DELIMS);
        strtok(NULL, DELIMS);

        // Read ammount, color pairs
        for (char* ammountString = strtok(NULL, DELIMS); ammountString != NULL; ammountString = strtok(NULL, DELIMS))
        {
            const int ammount = atoi(ammountString);
            if (ammount == 0)
            {
                fprintf(stderr, "Invalid ammount \"%s\" on line %d\n", ammountString, lineNum);
                goto cleanup;
            }

            const char* const colorString = strtok(NULL, DELIMS);
            if (colorString == NULL)
            {
                fprintf(stderr, "Expected color on line %d\n", lineNum);
                goto cleanup;
            }

            // If ammount is greater than current max for given color record new max
            if (strcmp(colorString, "red") == 0)
            {
                minNeeded[0] = ammount > minNeeded[0] ? ammount : minNeeded[0];
            }
            else if (strcmp(colorString, "green") == 0)
            {
                minNeeded[1] = ammount > minNeeded[1] ? ammount : minNeeded[1];
            }
            else if (strcmp(colorString, "blue") == 0)
            {
                minNeeded[2] = ammount > minNeeded[2] ? ammount : minNeeded[2];
            }
            else
            {
                fprintf(stderr, "Unknown color \"%s\" on line %d\n", colorString, lineNum);
                goto cleanup;
            }
        }

        // Check if game is possible in regards to all 3 colors
        bool possible = true;
        for (size_t i = 0; i < NUM_COLORS; ++i)
        {
            if (minNeeded[i] > MAX_CUBES[i])
            {
                possible = false;
                break;
            }
        }

        // Add line ID to total if game is possible
        if (possible)
            idSum += lineNum;
    }

    printf("Result: %d\n", idSum);

cleanup:
    lineBufferDestroy(&line);

    if (fclose(infile) == EOF)
        fputs("fclose() failed\n", stderr);

    return 0;
}