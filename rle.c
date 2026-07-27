#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "rle.h"

EngineStatus rleEncode(const char text[], char output[], size_t outputCapacity)
{
    size_t i = 0, k = 0;

    if (text == NULL || output == NULL || outputCapacity == 0)
        return ENGINE_INVALID_ARGUMENT;

    while (text[i] != '\0')
    {
        char ch = text[i];
        size_t count = 1;

        while (text[i + 1] == ch)
        {
            count++;
            i++;
        }

        int written;

        if (k + 1 >= outputCapacity)
            return ENGINE_OUTPUT_TOO_SMALL;
        output[k++] = ch;
        written = snprintf(output + k, outputCapacity - k, "%zu", count);
        if (written < 0 || (size_t)written >= outputCapacity - k)
            return ENGINE_OUTPUT_TOO_SMALL;
        k += (size_t)written;

        i++;
    }

    output[k] = '\0';
    return ENGINE_OK;
}

EngineStatus rleDecode(const char encoded[], char output[], size_t outputCapacity)
{
    size_t i = 0, k = 0;

    if (encoded == NULL || output == NULL || outputCapacity == 0)
        return ENGINE_INVALID_ARGUMENT;

    while (encoded[i] != '\0')
    {
        char ch = encoded[i++];
        size_t count = 0;

        while (encoded[i] >= '0' && encoded[i] <= '9')
        {
            if (count > (SIZE_MAX - (size_t)(encoded[i] - '0')) / 10U)
                return ENGINE_MALFORMED_INPUT;
            count = count * 10U + (size_t)(encoded[i] - '0');
            i++;
        }

        if (count == 0 || count >= outputCapacity - k)
            return count == 0 ? ENGINE_MALFORMED_INPUT : ENGINE_OUTPUT_TOO_SMALL;

        for (size_t j = 0; j < count; j++)
            output[k++] = ch;
    }

    output[k] = '\0';
    return ENGINE_OK;
}

size_t rleBits(const char encoded[])
{
    return strlen(encoded) * 8;
}
