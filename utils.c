#include <string.h>
#include "utils.h"

EngineStatus encodeText(const char text[], const HuffmanCodeTable codes,
                        char encoded[], size_t encodedCapacity)
{
    size_t encodedLength = 0;

    if (text == NULL || codes == NULL || encoded == NULL || encodedCapacity == 0)
        return ENGINE_INVALID_ARGUMENT;

    encoded[0] = '\0';

    for (int i = 0; text[i] != '\0'; i++)
    {
        const char *code = codes[(unsigned char)text[i]];
        size_t codeLength = strlen(code);

        if (codeLength == 0)
            return ENGINE_MALFORMED_INPUT;
        if (codeLength >= encodedCapacity - encodedLength)
            return ENGINE_OUTPUT_TOO_SMALL;

        memcpy(encoded + encodedLength, code, codeLength + 1);
        encodedLength += codeLength;
    }

    return ENGINE_OK;
}
