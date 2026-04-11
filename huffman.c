#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "huffman.h"

Node *createNode(char ch, int freq)
{
    Node *node = (Node *)malloc(sizeof(Node));
    node->ch = ch;
    node->freq = freq;
    node->left = node->right = NULL;
    return node;
}

void swap(Node **a, Node **b)
{
    Node *temp = *a;
    *a = *b;
    *b = temp;
}

void heapifyUp(MinHeap *heap, int index)
{
    while (index > 0)
    {
        int parent = (index - 1) / 2;

        if (heap->arr[parent]->freq <= heap->arr[index]->freq)
            break;

        swap(&heap->arr[parent], &heap->arr[index]);
        index = parent;
    }
}

void heapifyDown(MinHeap *heap, int index)
{
    int smallest = index;

    while (1)
    {
        int left = 2 * index + 1;
        int right = 2 * index + 2;

        if (left < heap->size &&
            heap->arr[left]->freq < heap->arr[smallest]->freq)
            smallest = left;

        if (right < heap->size &&
            heap->arr[right]->freq < heap->arr[smallest]->freq)
            smallest = right;

        if (smallest == index)
            break;

        swap(&heap->arr[index], &heap->arr[smallest]);
        index = smallest;
    }
}
void insertHeap(MinHeap *heap, Node *node)
{
    heap->arr[heap->size] = node;
    heapifyUp(heap, heap->size);
    heap->size++;
}

Node *extractMin(MinHeap *heap)
{
    if (heap->size == 0)
        return NULL;

    Node *min = heap->arr[0];
    heap->arr[0] = heap->arr[heap->size - 1];
    heap->size--;

    heapifyDown(heap, 0);

    return min;
}

Node *buildHuffmanTree(int freq[])
{
    MinHeap heap;
    heap.size = 0;

    for (int i = 0; i < MAX; i++)
    {
        if (freq[i] > 0)
            insertHeap(&heap, createNode(i, freq[i]));
    }

    while (heap.size > 1)
    {
        Node *left = extractMin(&heap);
        Node *right = extractMin(&heap);

        Node *newNode = createNode('$', left->freq + right->freq);
        newNode->left = left;
        newNode->right = right;

        insertHeap(&heap, newNode);
    }

    return extractMin(&heap);
}

void generateCodes(Node *root, char *code, int depth, char codes[256][100])
{
    if (!root)
        return;

    if (!root->left && !root->right)
    {
        code[depth] = '\0';
        printf("%c -> %s\n", root->ch, code);
        strcpy(codes[(unsigned char)root->ch], code);
        return;
    }

    code[depth] = '0';
    generateCodes(root->left, code, depth + 1, codes);

    code[depth] = '1';
    generateCodes(root->right, code, depth + 1, codes);
}

void printTree(Node *root, int level, char *prefix, int isLeft)
{
    if (!root)
        return;

    char newPrefix[1000];

    if (root->right)
    {
        snprintf(newPrefix, sizeof(newPrefix), "%s%s", prefix, isLeft ? "│   " : "    ");
        printTree(root->right, level + 1, newPrefix, 0);
    }

    printf("%s", prefix);

    if (level != 0)
        printf(isLeft ? "└── " : "┌── ");

    if (!root->left && !root->right)
        printf("%c(%d)\n", root->ch, root->freq);
    else
        printf("INT(%d)\n", root->freq);

    if (root->left)
    {
        snprintf(newPrefix, sizeof(newPrefix), "%s%s", prefix, isLeft ? "    " : "│   ");
        printTree(root->left, level + 1, newPrefix, 1);
    }
}

void freeTree(Node *root)
{
    if (!root)
        return;
    freeTree(root->left);
    freeTree(root->right);
    free(root);
}