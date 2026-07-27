#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <stddef.h>
#include "engine.h"

#define MAX ASE_SYMBOL_COUNT
typedef char HuffmanCodeTable[ASE_SYMBOL_COUNT][ASE_HUFFMAN_CODE_CAPACITY];

typedef struct Node
{
    char ch;
    int freq;
    struct Node *left, *right;
} Node;

Node *buildHuffmanTree(const int freq[], EngineStatus *status);
EngineStatus generateCodes(const Node *root, HuffmanCodeTable codes);
void freeTree(Node *root);
EngineStatus decodeHuffman(const Node *root, const char encoded[], char decoded[], size_t decodedCapacity);
EngineStatus getCompressedBits(const char text[], const HuffmanCodeTable codes, size_t *bits);
#endif
