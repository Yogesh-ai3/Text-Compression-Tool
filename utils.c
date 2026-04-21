#include <stdio.h>
#include <string.h>
#include "utils.h"
#include "huffman.h"

void calculateFrequency(char text[], int freq[])
{
    for (int i = 0; text[i] != '\0'; i++)
        freq[(unsigned char)text[i]]++;
}

void printFrequency(int freq[])
{
    printf("\nCharacter Frequencies:\n");
    for (int i = 0; i < MAX; i++)
    {
        if (freq[i] > 0)
            printf("%c -> %d\n", i, freq[i]);
    }
}

void encodeText(char text[], char codes[256][100], char encoded[])
{
    encoded[0] = '\0';

    for (int i = 0; text[i] != '\0'; i++)
        strcat(encoded, codes[(unsigned char)text[i]]);

    printf("\nEncoded String:\n%s\n", encoded);
}

void printStats(char text[], char codes[256][100])
{
    int original_bits = strlen(text) * 8;
    int compressed_bits = getCompressedBits(text, codes);
    printf("\nOriginal bits: %d\n", original_bits);
    printf("Compressed bits: %d\n", compressed_bits);
    printf("Compression ratio: %.2f\n",
           (float)compressed_bits / original_bits);
}