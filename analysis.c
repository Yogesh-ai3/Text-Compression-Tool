#include <string.h>
#include "analysis.h"

EngineStatus analyseText(const char text[], TextAnalysis *analysis)
{
    size_t currentRun = 0;
    unsigned char previous = 0;

    if (text == NULL || analysis == NULL)
        return ENGINE_INVALID_ARGUMENT;

    memset(analysis, 0, sizeof(*analysis));

    for (size_t i = 0; text[i] != '\0'; i++)
    {
        unsigned char symbol = (unsigned char)text[i];

        analysis->length++;
        if (analysis->frequency[symbol]++ == 0)
            analysis->uniqueCharacters++;

        currentRun = (i > 0 && symbol == previous) ? currentRun + 1 : 1;
        if (currentRun > analysis->maximumRunLength)
            analysis->maximumRunLength = currentRun;
        previous = symbol;
    }

    return ENGINE_OK;
}
