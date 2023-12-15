#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"

void lineBufferCreate(LineBuffer* const this)
{
    this->line = NULL;
    this->size = 0;
    this->capacity = 0;
}

void lineBufferDestroy(LineBuffer* const this)
{
    free(this->line);
}

size_t nextLineSize(FILE* const file)
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

bool readNextLine(FILE* const file, LineBuffer* const buffer)
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

size_t countLines(FILE* const file)
{
    // Record starting position to restore later
    const long int starting = ftell(file);
    if (starting == -1)
    {
        perror("ftell() failed");
        return (size_t)-1;
    }

    size_t count = 1;

    for (int c = fgetc(file); c != EOF; c = fgetc(file))
    {
        if (c == '\n')
            ++count;
    }

    // Restore starting file position
    if (fseek(file, starting, SEEK_SET) != 0)
        return (size_t)-1;

    return count;
}

void bstMapNodeCreate(struct BSTMapNode* const this, void* const key, void* const value)
{
    this->key = key;
    this->value = value;
    this->left = NULL;
    this->right = NULL;
}

void bstMapDestroyHelper(struct BSTMapNode* const node)
{
    if (node == NULL)
        return;

    bstMapDestroyHelper(node->left);
    bstMapDestroyHelper(node->right);

    free(node->key);
    free(node->value);
    free(node);
}

void bstMapDestroy(BSTMap* const this)
{
    bstMapDestroyHelper(this->root);
}

void bstMapCreate(BSTMap* const this, int(* const compare)(const void*, const void*))
{
    this->compare = compare;
    this->root = NULL;
}

static struct BSTMapNode* bstMapInsertHelper(const BSTMap* const tree, struct BSTMapNode* const node, void* const key, void* const value)
{
    if (node == NULL)
    {
        struct BSTMapNode* const newNode = malloc(sizeof(struct BSTMapNode));
        bstMapNodeCreate(newNode, key, value);
        return newNode;
    }

    const int comparision = tree->compare(key, node->key);

    if (comparision == 0)
    {
        free(key);
        free(value);
        return node;
    }
    else if (comparision < 0)
    {
        node->left = bstMapInsertHelper(tree, node->left, key, value);
        return node;
    }
    else
    {
        node->right = bstMapInsertHelper(tree, node->right, key, value);
        return node;
    }
}

void bstMapInsert(BSTMap* const this, void* const key, void* const value)
{
    this->root = bstMapInsertHelper(this, this->root, key, value);
}

static void* bstMapFindHelper(const BSTMap* const tree, struct BSTMapNode* const node, const void* const key)
{
    if (node == NULL)
        return NULL;

    const int comparision = tree->compare(key, node->key);

    if (comparision == 0)
    {
        return node->value;
    }
    else if (comparision < 0)
    {
        return bstMapFindHelper(tree, node->left, key);
    }
    else
    {
        return bstMapFindHelper(tree, node->right, key);
    }
}

void* bstMapFind(BSTMap* const this, const void* const key)
{
    return bstMapFindHelper(this, this->root, key);
}

static void bstMapForEachHelper(struct BSTMapNode* const node, void(* const func)(void*, void*, void*), void* const arg)
{
    if (node == NULL)
        return;
    
    func(node->key, node->value, arg);
    bstMapForEachHelper(node->left, func, arg);
    bstMapForEachHelper(node->right, func, arg);
}

void bstMapForEach(BSTMap* const this, void(* const func)(void*, void*, void*), void* const arg)
{
    bstMapForEachHelper(this->root, func, arg);
}

static size_t bstMapSizeHelper(const struct BSTMapNode* const node)
{
    if (node == NULL)
        return 0;
    
    return 1 + bstMapSizeHelper(node->left) + bstMapSizeHelper(node->right);
}

size_t bstMapSize(const BSTMap* const this)
{
    return bstMapSizeHelper(this->root);
}

void dynamicArrayCreate(DynamicArray* const this, const size_t elementSize)
{
    this->elementSize = elementSize;
    this->array = NULL;
    this->size = 0;
    this->capacity = 0;
}

void dynamicArrayDestroy(DynamicArray* const this)
{
    free(this->array);
}

const void* dynamicArrayIndex(const DynamicArray* const this, const size_t index)
{
    return (const char*)this->array + this->elementSize * index;
}

void dynamicArrayPushBack(DynamicArray* const this, const void* const elements, const size_t count)
{
    if (this->size == this->capacity)
    {
        while (this->capacity < this->size + count)
        {
            this->capacity = this->capacity > 0 ? this->capacity * 2 : 1;
        }

        this->array = realloc(this->array, this->capacity * this->elementSize);
    }

    memcpy((char*)this->array + this->size * this->elementSize, elements, this->elementSize * count);
    ++this->size;
}

void dynamicArrayClear(DynamicArray* const this)
{
    this->size = 0;
}