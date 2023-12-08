#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

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

        // Find first digit
        for (size_t i = 0; i < lineSize; ++i)
        {
            if (isdigit(buffer[i]))
            {
                first = buffer[i] - '0';
                break;
            }
        }

        // Error if no digit is found in line
        if (first == -1)
        {
            fprintf(stderr, "No digit found in line \"%s\"\n", buffer);
            goto cleanup;
        }

        // Find second digit
        for (size_t i = lineSize - 1; i != (size_t)-1; --i)
        {
            if (isdigit(buffer[i]))
            {
                last = buffer[i] - '0';
                break;
            }
        }

        // Add to total
        total += (first * 10) + last;
    }

    printf("Result: %d\n", total);

cleanup:
    if (fclose(infile) == EOF)
        fputs("fclose() failed", stderr);

    return 0;
}