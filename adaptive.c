#include <stdio.h>
#include <string.h>
#include "adaptive.h"

int chooseBest(char text[], int huffmanBits, int rleBits)
{
    int freq[256] = {0};
    int unique = 0;
    int maxRun = 1;
    int currentRun = 1;
    int len = strlen(text);

    int choice = 1; // 1 = Huffman, 2 = RLE
    const char *reason = "Smaller compressed size favored Huffman.";

    if (len == 0)
    {
        printf("\nAdaptive Analysis:\n");
        printf("Empty input detected.\n");
        printf("Chosen Method = Huffman\n");
        printf("Reason: Default fallback for empty input.\n");
        return 1;
    }

    /* Count unique characters */
    for (int i = 0; text[i] != '\0'; i++)
    {
        unsigned char ch = (unsigned char)text[i];

        if (freq[ch] == 0)
            unique++;

        freq[ch]++;
    }

    /* Find maximum consecutive repetition streak */
    for (int i = 1; text[i] != '\0'; i++)
    {
        if (text[i] == text[i - 1])
        {
            currentRun++;

            if (currentRun > maxRun)
                maxRun = currentRun;
        }
        else
        {
            currentRun = 1;
        }
    }

    /* Decision Rules with explicit reason tracking */

    if (unique == 1)
    {
        choice = 2;
        reason = "Only one unique character detected.";
    }
    else if (maxRun >= 4)
    {
        choice = 2;
        reason = "Long repetition streak detected.";
    }
    else if (unique <= len / 4 && len > 8)
    {
        choice = 2;
        reason = "Low symbol diversity suggests repetition-heavy data.";
    }
    else if (rleBits < huffmanBits)
    {
        choice = 2;
        reason = "Measured compressed size favored RLE.";
    }
    else if (huffmanBits < rleBits)
    {
        choice = 1;
        reason = "Measured compressed size favored Huffman.";
    }
    else
    {
        choice = 1;
        reason = "Tie detected; Huffman selected as default.";
    }

    /* Transparent diagnostics */

    printf("\nAdaptive Analysis:\n");

    printf("\nInput Length = %d\n", len);

    printf("\nUnique Characters = %d\n", unique);

    printf("\nMax Repetition Streak = %d\n", maxRun);                    

    printf("\nHuffman Bits = %d\n", huffmanBits);

    printf("\nRLE Bits = %d\n", rleBits);

    printf("\nChosen Method = %s\n", choice == 1 ? "Huffman" : "RLE");

    printf("\nReason: %s\n", reason);

    return choice;
}