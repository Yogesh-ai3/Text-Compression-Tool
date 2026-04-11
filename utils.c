#include <stdio.h>
#include <string.h>
#include "utils.h"

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

void encodeText(char text[], char codes[256][100])
{
    printf("\nEncoded String:\n");
    for (int i = 0; text[i] != '\0'; i++)
        printf("%s", codes[(unsigned char)text[i]]);
    printf("\n");
}

void printStats(char text[], char codes[256][100])
{
    int original_bits = strlen(text) * 8;
    int compressed_bits = 0;

    for (int i = 0; text[i] != '\0'; i++)
        compressed_bits += strlen(codes[(unsigned char)text[i]]);

    printf("\nOriginal bits: %d\n", original_bits);
    printf("Compressed bits: %d\n", compressed_bits);
    printf("Compression ratio: %.2f\n",
           (float)compressed_bits / original_bits);
}