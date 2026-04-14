#include <stdio.h>
#include <string.h>
#include "huffman.h"
#include "utils.h"

int main()
{
    char text[1000];
    char codes[256][100] = {{0}};
    char code[100];
    int freq[MAX] = {0};

    printf("Enter text: ");
    scanf("%s", text);

    calculateFrequency(text, freq);
    printFrequency(freq);

    Node *root = buildHuffmanTree(freq);

    printf("\nHuffman Codes:\n");
    generateCodes(root, code, 0, codes);

    encodeText(text, codes);
    printStats(text, codes);

    printf("\nHuffman Tree:\n");
    printTree(root, 0, "", 1);

    freeTree(root);
    return 0;
}