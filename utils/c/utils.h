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
size_t bstMapSize(const BSTMap* this);

typedef struct
{
    size_t elementSize;

    void* array;
    size_t size;
    size_t capacity;
} DynamicArray;

void dynamicArrayCreate(DynamicArray* this, size_t elementSize);
void dynamicArrayDestroy(DynamicArray* this);
const void* dynamicArrayIndex(const DynamicArray* this, size_t index);
void dynamicArrayPushBack(DynamicArray* this, const void* elements, size_t count);
void dynamicArrayResize(DynamicArray* this, size_t size);
void dynamicArrayClear(DynamicArray* this);
void dynamicArrayErase(DynamicArray* this, size_t index);

#endif
