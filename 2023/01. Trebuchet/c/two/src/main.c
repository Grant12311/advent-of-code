#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const char* const DIGIT_STRINGS[] = {
    "one",
    "two",
    "three",
    "four",
    "five",
    "six",
    "seven",
    "eight",
    "nine"
};

static size_t DIGIT_STRING_SIZES[] = {
    3,
    3,
    5,
    4,
    4,
    3,
    5,
    5,
    4
};

/**
 * @brief Scan forward to find length of current line of file
 * 
 * @param file 
 * @return size_t Number of characters before newline or EOF was found
 */
static size_t nextLineSize(FILE* const file)
{
    // Record starting position to restore later
    const long int starting = ftell(file);
    if (starting == -1)
    {
        perror("ftell() failed");
        return (size_t)-1;
    }

    size_t size = 0;
    
    // Count characters until newline or EOF
    for (int c = fgetc(file); c != '\n' && c != EOF; c = fgetc(file))
    {
        ++size;
    }

    // Restore starting file position
    if (fseek(file, starting, SEEK_SET) != 0)
        return (size_t)-1;
    
    return size;
}

/**
 * @brief Find last occurence of substring in string
 * 
 * @param str 
 * @param strSize 
 * @param substr 
 * @param substrSize 
 * @return const char* Pointer to last occurence or NULL
 */
static const char* findLastSubstring(const char* const str, const size_t strSize, const char* const substr, const size_t substrSize)
{    
    for (const char* ptr = str + strSize - substrSize; ptr >= str; --ptr)
    {
        if (strncmp(ptr, substr, substrSize) == 0)
            return ptr;
    }

    return NULL;
}

int main(const int argc, const char** const argv)
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <INPUT_FILE>\n", argv[0]);
        return 1;
    }

    FILE* const infile = fopen(argv[1], "r");

    // Buffer to store each line
    char* buffer = NULL;
    size_t bufferSize = 0;

    int total = 0;

    // Read each line of file
    for (size_t lineSize = nextLineSize(infile); lineSize > 0; lineSize = nextLineSize(infile))
    {
        // Grow buffer to fit line if needed
        if (bufferSize < lineSize + 1)
        {
            buffer = realloc(buffer, lineSize + 1);
            bufferSize = lineSize + 1;
        }

        // Read line
        fread(buffer, sizeof(char), lineSize + 1, infile);
        buffer[lineSize] = '\0';

        int first = -1;
        int last = -1;

        const char* firstPtr = NULL;
        const char* lastPtr = NULL;

        // Find first digit
        for (size_t i = 0; i < lineSize; ++i)
        {
            if (isdigit(buffer[i]))
            {
                first = buffer[i] - '0';
                firstPtr = buffer + i;
                break;
            }
        }

        // Find first digit word
        for (size_t i = 0; i < sizeof(DIGIT_STRINGS) / sizeof(*DIGIT_STRINGS); ++i)
        {
            const char* const found = strstr(buffer, DIGIT_STRINGS[i]);
            if (found != NULL && (firstPtr == NULL || found < firstPtr))
            {
                first = (int)i + 1;
                firstPtr = found;
            }
        }

        // Find last digit
        for (size_t i = lineSize - 1; i != (size_t)-1; --i)
        {
            if (isdigit(buffer[i]))
            {
                last = buffer[i] - '0';
                lastPtr = buffer + i;
                break;
            }
        }

        // Find last digit word
        for (size_t i = 0; i < sizeof(DIGIT_STRINGS) / sizeof(*DIGIT_STRINGS); ++i)
        {
            const char* const found = findLastSubstring(buffer, lineSize, DIGIT_STRINGS[i], DIGIT_STRING_SIZES[i]);
            if (found != NULL && (lastPtr == NULL || found > lastPtr))
            {
                last = (int)i + 1;
                lastPtr = found;
            }
        }

        // If the digits are numeric digits convert them to int
        if (isdigit(*firstPtr))
            first = *firstPtr - '0';
        if (isdigit(*lastPtr))
            last = *lastPtr - '0';

        // Add to total
        total += (first * 10) + last;
    }

    printf("Result: %d\n", total);

    if (fclose(infile) == EOF)
        fputs("fclose() failed", stderr);

    return 0;
}