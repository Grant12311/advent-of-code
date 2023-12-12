#ifndef AOC_C_UTILS_H
#define AOC_C_UTILS_H

#include <stdbool.h>

typedef struct
{
    char* line;
    size_t size;
    size_t capacity;
} LineBuffer;

void lineBufferCreate(LineBuffer* this);
void lineBufferDestroy(LineBuffer* this);
bool readNextLine(FILE* file, LineBuffer* buffer);

size_t countLines(FILE* file);

struct BSTMapNode
{
    void* key;
    void* value;
    struct BSTMapNode* left;
    struct BSTMapNode* right;
};

void bstMapNodeCreate(struct BSTMapNode* this, void* key, void* value);

typedef struct
{
    int(*compare)(const void*, const void*);

    struct BSTMapNode* root;
} BSTMap;

void bstMapCreate(BSTMap* this, int(*compare)(const void*, const void*));
void bstMapDestroy(BSTMap* this);
void bstMapInsert(BSTMap* this, void* key, void* value);
void* bstMapFind(BSTMap* this, const void* key);
void bstMapForEach(BSTMap* this, void(*func)(void*, void*, void*), void* arg);

#endif
