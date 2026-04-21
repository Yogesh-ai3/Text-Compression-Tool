#ifndef HUFFMAN_H
#define HUFFMAN_H

#define MAX 256

typedef struct Node
{
    char ch;
    int freq;
    struct Node *left, *right;
} Node;

typedef struct
{
    Node *arr[MAX];
    int size;
} MinHeap;

// Core functions
Node *buildHuffmanTree(int freq[]);
void generateCodes(Node *root, char *code, int depth, char codes[256][100]);
void printTree(Node *root, int level, char *prefix, int isLeft);
void freeTree(Node *root);
void decodeHuffman(Node *root, char encoded[]);
int getCompressedBits(char text[], char codes[256][100]);
void decodeHuffman(Node *root, char encoded[]);
#endif