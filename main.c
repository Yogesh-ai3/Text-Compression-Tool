#include <stdio.h>
#include <string.h>
#include "huffman.h"
#include "utils.h"
#include "rle.h"
#include "adaptive.h"

int main()
{
    char text[1000];
    char codes[256][100] = {{0}};
    char code[100];
    int freq[MAX] = {0};
    char encoded[10000];

    char rleOutput[2000];
    char decodedRLE[1000];

    printf("Enter text: ");
    fgets(text, sizeof(text), stdin);
    text[strcspn(text, "\n")] = '\0';

    calculateFrequency(text, freq);
    printFrequency(freq);

    Node *root = buildHuffmanTree(freq);

    printf("\nHuffman Codes:\n");
    generateCodes(root, code, 0, codes);

    encodeText(text, codes, encoded);
    decodeHuffman(root, encoded);
    printStats(text, codes);
    printf("\nHuffman Tree:\n");
    printTree(root, 0, "", 1);
    int huffmanBits = getCompressedBits(text, codes);

    printf("\nHuffman Bits: %d\n", huffmanBits);

    rleEncode(text, rleOutput);
    printf("\nRLE Encoded:%s\n", rleOutput);

    int rlebits = rleBits(rleOutput);
    printf("RLE Bits: %d\n", rlebits);

    int best = chooseBest(text, huffmanBits, rlebits);

    if (best == 1)
        printf("\nBest Compression Method: Huffman\n");
    else
        printf("\nBest Compression Method: RLE\n");

    rleDecode(rleOutput, decodedRLE);
    printf("\nRLE Decoded:\n%s\n", decodedRLE);

    freeTree(root);

    return 0;
}