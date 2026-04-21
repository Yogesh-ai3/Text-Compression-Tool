#include <stdio.h>
#include <string.h>
#include "rle.h"

void rleEncode(char text[], char output[])
{
    int i = 0, k = 0;

    while (text[i] != '\0')
    {
        char ch = text[i];
        int count = 1;

        while (text[i + 1] == ch)
        {
            count++;
            i++;
        }

        output[k++] = ch;
        k += sprintf(&output[k], "%d", count);

        i++;
    }

    output[k] = '\0';
}

void rleDecode(char encoded[], char output[])
{
    int i = 0, k = 0;

    while (encoded[i] != '\0')
    {
        char ch = encoded[i++];
        int count = 0;

        while (encoded[i] >= '0' && encoded[i] <= '9')
        {
            count = count * 10 + (encoded[i] - '0');
            i++;
        }

        for (int j = 0; j < count; j++)
            output[k++] = ch;
    }

    output[k] = '\0';
}

int rleBits(char encoded[])
{
    return strlen(encoded) * 8;
}