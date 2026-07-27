#include <stdio.h>
#include <string.h>
#include "huffman.h"
#include "analysis.h"
#include "utils.h"
#include "rle.h"
#include "adaptive.h"

enum
{
    INPUT_CAPACITY = 1000,
    RLE_OUTPUT_CAPACITY = INPUT_CAPACITY * 5
};

static void printFrequency(const int freq[])
{
    printf("\nCharacter Frequencies:\n");
    for (size_t i = 0; i < MAX; i++)
    {
        if (freq[i] > 0)
            printf("%c -> %d\n", (unsigned char)i, freq[i]);
    }
}

static void printHuffmanCodes(const HuffmanCodeTable codes)
{
    printf("\nHuffman Codes:\n");
    for (size_t i = 0; i < MAX; i++)
    {
        if (codes[i][0] != '\0')
            printf("%c -> %s\n", (unsigned char)i, codes[i]);
    }
}

static void printHuffmanTree(const Node *root, int level, const char *prefix, int isLeft)
{
    char newPrefix[1000];

    if (!root)
        return;

    if (root->right)
    {
        snprintf(newPrefix, sizeof(newPrefix), "%s%s", prefix, isLeft ? "|   " : "    ");
        printHuffmanTree(root->right, level + 1, newPrefix, 0);
    }

    printf("%s", prefix);
    if (level != 0)
        printf(isLeft ? "`-- " : "/-- ");
    printf(!root->left && !root->right ? "%c(%d)\n" : "INT(%d)\n", root->ch, root->freq);

    if (root->left)
    {
        snprintf(newPrefix, sizeof(newPrefix), "%s%s", prefix, isLeft ? "    " : "|   ");
        printHuffmanTree(root->left, level + 1, newPrefix, 1);
    }
}

static void printStats(const char text[], size_t compressedBits)
{
    size_t originalBits = strlen(text) * 8U;

    printf("\nOriginal bits: %zu\n", originalBits);
    printf("Compressed bits: %zu\n", compressedBits);
    if (originalBits == 0)
        printf("Compression ratio: n/a (empty input)\n");
    else
        printf("Compression ratio: %.2f\n", (double)compressedBits / (double)originalBits);
}

static void printAdaptiveDecision(const AdaptiveDecision *decision)
{
    printf("\nAdaptive Analysis:\n");
    printf("\nInput Length = %zu\n", decision->inputLength);
    printf("\nUnique Characters = %zu\n", decision->uniqueCharacters);
    printf("\nMax Repetition Streak = %zu\n", decision->maximumRunLength);
    printf("\nHuffman Bits = %zu\n", decision->huffmanBits);
    printf("\nRLE Bits = %zu\n", decision->rleBits);
    printf("\nChosen Method = %s\n",
           decision->method == COMPRESSION_HUFFMAN ? "Huffman" : "RLE");
    printf("\nReason: %s\n", decision->reason);
}

static int reportFailure(const char *operation, EngineStatus status)
{
    fprintf(stderr, "%s failed: %s\n", operation, engineStatusMessage(status));
    return 1;
}

int main(void)
{
    char text[INPUT_CAPACITY];
    HuffmanCodeTable codes = {{0}};
    char encoded[INPUT_CAPACITY * ASE_HUFFMAN_CODE_CAPACITY];
    char decodedHuffman[INPUT_CAPACITY];

    char rleOutput[RLE_OUTPUT_CAPACITY];
    char decodedRLE[INPUT_CAPACITY];
    TextAnalysis analysis;
    EngineStatus status;
    size_t huffmanBits;
    size_t rlebits;

    printf("Enter text: ");
    if (fgets(text, sizeof(text), stdin) == NULL)
        return reportFailure("Input", ENGINE_INVALID_ARGUMENT);
    text[strcspn(text, "\n")] = '\0';

    status = analyseText(text, &analysis);
    if (status != ENGINE_OK)
        return reportFailure("Text analysis", status);
    printFrequency(analysis.frequency);

    Node *root = buildHuffmanTree(analysis.frequency, &status);
    if (status != ENGINE_OK)
        return reportFailure("Huffman tree construction", status);

    if (root == NULL)
    {
        printf("\nNo compression output for empty input.\n");
        return 0;
    }

    status = generateCodes(root, codes);
    if (status != ENGINE_OK)
    {
        freeTree(root);
        return reportFailure("Huffman code generation", status);
    }
    printHuffmanCodes(codes);

    status = encodeText(text, codes, encoded, sizeof(encoded));
    if (status != ENGINE_OK)
    {
        freeTree(root);
        return reportFailure("Huffman encoding", status);
    }
    printf("\nEncoded String:\n%s\n", encoded);
    status = decodeHuffman(root, encoded, decodedHuffman, sizeof(decodedHuffman));
    if (status != ENGINE_OK)
    {
        freeTree(root);
        return reportFailure("Huffman decoding", status);
    }
    printf("\nHuffman Decoded:\n%s\n", decodedHuffman);
    status = getCompressedBits(text, codes, &huffmanBits);
    if (status != ENGINE_OK)
    {
        freeTree(root);
        return reportFailure("Huffman statistics", status);
    }
    printStats(text, huffmanBits);
    printf("\nHuffman Tree:\n");
    printHuffmanTree(root, 0, "", 1);
    printf("\nHuffman Bits: %zu\n", huffmanBits);

    status = rleEncode(text, rleOutput, sizeof(rleOutput));
    if (status != ENGINE_OK)
    {
        freeTree(root);
        return reportFailure("RLE encoding", status);
    }
    printf("\nRLE Encoded:%s\n", rleOutput);

    rlebits = rleBits(rleOutput);
    printf("RLE Bits: %zu\n", rlebits);

    AdaptiveDecision decision = chooseBest(&analysis, huffmanBits, rlebits);
    printAdaptiveDecision(&decision);

    if (decision.method == COMPRESSION_HUFFMAN)
        printf("\nBest Compression Method: Huffman\n");
    else
        printf("\nBest Compression Method: RLE\n");

    status = rleDecode(rleOutput, decodedRLE, sizeof(decodedRLE));
    if (status != ENGINE_OK)
    {
        freeTree(root);
        return reportFailure("RLE decoding", status);
    }
    printf("\nRLE Decoded:\n%s\n", decodedRLE);

    freeTree(root);

    return 0;
}
