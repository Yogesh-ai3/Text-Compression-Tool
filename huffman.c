#include <stdlib.h>
#include <string.h>
#include "huffman.h"

typedef struct
{
    Node *arr[ASE_SYMBOL_COUNT];
    size_t size;
} MinHeap;

static Node *createNode(char ch, int freq)
{
    Node *node = (Node *)malloc(sizeof(Node));
    if (node == NULL)
        return NULL;
    node->ch = ch;
    node->freq = freq;
    node->left = node->right = NULL;
    return node;
}

static void swap(Node **a, Node **b)
{
    Node *temp = *a;
    *a = *b;
    *b = temp;
}

static void heapifyUp(MinHeap *heap, size_t index)
{
    while (index > 0)
    {
        size_t parent = (index - 1) / 2;

        if (heap->arr[parent]->freq <= heap->arr[index]->freq)
            break;

        swap(&heap->arr[parent], &heap->arr[index]);
        index = parent;
    }
}

static void heapifyDown(MinHeap *heap, size_t index)
{
    size_t smallest = index;

    while (1)
    {
        size_t left = 2 * index + 1;
        size_t right = 2 * index + 2;

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
static void insertHeap(MinHeap *heap, Node *node)
{
    heap->arr[heap->size] = node;
    heapifyUp(heap, heap->size);
    heap->size++;
}

static Node *extractMin(MinHeap *heap)
{
    if (heap->size == 0)
        return NULL;

    Node *min = heap->arr[0];
    heap->arr[0] = heap->arr[heap->size - 1];
    heap->size--;

    heapifyDown(heap, 0);

    return min;
}

Node *buildHuffmanTree(const int freq[], EngineStatus *status)
{
    MinHeap heap;
    heap.size = 0;

    if (freq == NULL || status == NULL)
        return NULL;

    *status = ENGINE_OK;

    for (size_t i = 0; i < ASE_SYMBOL_COUNT; i++)
    {
        if (freq[i] > 0)
        {
            Node *node = createNode((char)i, freq[i]);
            if (node == NULL)
            {
                while (heap.size > 0)
                    freeTree(extractMin(&heap));
                *status = ENGINE_ALLOCATION_FAILED;
                return NULL;
            }
            insertHeap(&heap, node);
        }
    }

    while (heap.size > 1)
    {
        Node *left = extractMin(&heap);
        Node *right = extractMin(&heap);

        Node *newNode = createNode('$', left->freq + right->freq);
        if (newNode == NULL)
        {
            freeTree(left);
            freeTree(right);
            while (heap.size > 0)
                freeTree(extractMin(&heap));
            *status = ENGINE_ALLOCATION_FAILED;
            return NULL;
        }
        newNode->left = left;
        newNode->right = right;

        insertHeap(&heap, newNode);
    }

    return extractMin(&heap);
}

static EngineStatus generateCodesRecursive(const Node *root, char code[], size_t depth,
                                           HuffmanCodeTable codes)
{
    if (!root)
        return ENGINE_OK;

    if (!root->left && !root->right)
    {
        if (depth == 0)
        {
            code[0] = '0';
            code[1] = '\0';
        }
        else
        {
            code[depth] = '\0';
        }

        strcpy(codes[(unsigned char)root->ch], code);
        return ENGINE_OK;
    }

    if (depth + 1 >= ASE_HUFFMAN_CODE_CAPACITY)
        return ENGINE_MALFORMED_INPUT;

    code[depth] = '0';
    EngineStatus status = generateCodesRecursive(root->left, code, depth + 1, codes);
    if (status != ENGINE_OK)
        return status;

    code[depth] = '1';
    return generateCodesRecursive(root->right, code, depth + 1, codes);
}

EngineStatus generateCodes(const Node *root, HuffmanCodeTable codes)
{
    char code[ASE_HUFFMAN_CODE_CAPACITY];

    if (root == NULL || codes == NULL)
        return ENGINE_INVALID_ARGUMENT;
    memset(codes, 0, sizeof(HuffmanCodeTable));
    return generateCodesRecursive(root, code, 0, codes);
}

EngineStatus getCompressedBits(const char text[], const HuffmanCodeTable codes, size_t *bits)
{
    if (text == NULL || codes == NULL || bits == NULL)
        return ENGINE_INVALID_ARGUMENT;

    *bits = 0;

    for (size_t i = 0; text[i] != '\0'; i++)
    {
        size_t codeLength = strlen(codes[(unsigned char)text[i]]);
        if (codeLength == 0)
            return ENGINE_MALFORMED_INPUT;
        *bits += codeLength;
    }

    return ENGINE_OK;
}
EngineStatus decodeHuffman(const Node *root, const char encoded[], char decoded[], size_t decodedCapacity)
{
    size_t decodedIndex = 0;

    if (root == NULL || encoded == NULL || decoded == NULL || decodedCapacity == 0)
        return ENGINE_INVALID_ARGUMENT;

    // Single character tree
    if (!root->left && !root->right)
    {
        for (int i = 0; encoded[i] != '\0'; i++)
        {
            if (encoded[i] != '0' || decodedIndex + 1 >= decodedCapacity)
                return encoded[i] != '0' ? ENGINE_MALFORMED_INPUT : ENGINE_OUTPUT_TOO_SMALL;
            decoded[decodedIndex++] = root->ch;
        }

        decoded[decodedIndex] = '\0';
        return ENGINE_OK;
    }

    const Node *curr = root;

    for (int i = 0; encoded[i] != '\0'; i++)
    {
        if (encoded[i] == '0')
            curr = curr->left;
        else if (encoded[i] == '1')
            curr = curr->right;
        else
            return ENGINE_MALFORMED_INPUT;

        if (curr == NULL)
            return ENGINE_MALFORMED_INPUT;

        if (!curr->left && !curr->right)
        {
            if (decodedIndex + 1 >= decodedCapacity)
                return ENGINE_OUTPUT_TOO_SMALL;
            decoded[decodedIndex++] = curr->ch;
            curr = root;
        }
    }

    if (curr != root)
        return ENGINE_MALFORMED_INPUT;
    decoded[decodedIndex] = '\0';
    return ENGINE_OK;
}
void freeTree(Node *root)
{
    if (!root)
        return;
    freeTree(root->left);
    freeTree(root->right);
    free(root);
}
