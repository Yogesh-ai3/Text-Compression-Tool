#ifndef ANALYSIS_H
#define ANALYSIS_H

#include <stddef.h>
#include "engine.h"

typedef struct
{
    size_t length;
    size_t uniqueCharacters;
    size_t maximumRunLength;
    int frequency[ASE_SYMBOL_COUNT];
} TextAnalysis;

EngineStatus analyseText(const char text[], TextAnalysis *analysis);

#endif
