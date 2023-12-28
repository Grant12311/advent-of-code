#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "utils.h"

typedef struct
{
    long long int time;
    long long int distance;
} Race;

/**
 * @brief Insert a series of time values from infile in the format of "Time: 7 15 30" into races
 *        Only the "time" fields of each race will be set. Distances will all be zero
 * 
 * @param infile 
 * @param races 
 * @return true if read was successful
 */
static bool readTimes(FILE* const infile, DynamicArray* const races)
{
    int count = -1;
    fscanf(infile, "Time: %n", &count);

    if (count == -1)
        return false;

    for (long long int time; fscanf(infile, "%lld", &time) == 1;)
    {
        Race race = {.time = time, .distance = 0};
        dynamicArrayPushBack(races, &race, 1);
    }

    return true;
}

/**
 * @brief Read a series of distance values from infile in the format of "Distance: 7 15 30" into races
 *        Values will be read until either input runs out of distances or races runs out of races
 * 
 * @param infile 
 * @param races 
 * @return true if read was successful
 */
static bool readDistances(FILE* const infile, DynamicArray* const races)
{
    int count = -1;
    fscanf(infile, "Distance: %n", &count);

    if (count == -1)
        return false;

    for (size_t i = 0; i < races->size && fscanf(infile, "%lld", &((Race*)dynamicArrayIndex(races, i))->distance) == 1; ++i);

    return true;
}

/**
 * @brief Read race time and distance values from infile in the format of
 *        Time:      7  15   30
*         Distance:  9  40  200
 * 
 * @param infile 
 * @return DynamicArray of Race
 */
static DynamicArray readRaces(FILE* const infile)
{
    DynamicArray races;
    dynamicArrayCreate(&races, sizeof(Race));

    readTimes(infile, &races);
    readDistances(infile, &races);

    return races;
}

/**
 * @brief Calcualte the number of possible ways to win race
 * 
 * @param race 
 * @return unsigned long long int 
 */
static unsigned long long int winMethods(const Race race)
{
    unsigned long long int methods = 0;

    // After the first win, any loss means we can't hold for any longer and still win, so abort
    bool firstWin = false;
    for (long long int hold = 1; hold <= race.time; ++hold)
    {
        if ((race.time - hold) * hold > race.distance)
        {
            firstWin = true;
            ++methods;
        }
        else if (firstWin)
        {
            break;
        }
    }

    return methods;
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

    DynamicArray races = readRaces(infile);

    // Product of all the win method counts
    unsigned long long int result = 1;

    for (size_t i = 0; i < races.size; ++i)
    {
        const Race* const race = dynamicArrayIndex(&races, i);
        result *= winMethods(*race);
    }

    printf("Result: %llu\n", result);

    dynamicArrayDestroy(&races);
    fclose(infile);

    return 0;
}