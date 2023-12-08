#ifndef AOC_C_UTILS_H
#define AOC_C_UTILS_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct
{
    char* line;
    size_t size;
    size_t capacity;
} LineBuffer;

static void lineBufferCreate(LineBuffer* const this)
{
    this->line = NULL;
    this->size = 0;
    this->capacity = 0;
}

static void lineBufferDestroy(LineBuffer* const this)
{
    free(this->line);
}

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

static bool readNextLine(FILE* const file, LineBuffer* const buffer)
{
    const size_t needed = nextLineSize(file);
    if (needed == (size_t)-1)
        return false;
    
    if (buffer->capacity < needed + 1)
    {
        buffer->line = realloc(buffer->line, sizeof(char) * needed + 1);
        buffer->size = needed;
        buffer->capacity = needed + 1;
    }

    if (fread(buffer->line, sizeof(char), needed + 1, file) != needed + 1)
        return false;

    buffer->line[needed] = '\0';
    
    return true;
}

#endif
