#ifndef ADAPTIVE_H
#define ADAPTIVE_H

#include <stddef.h>
#include "analysis.h"

typedef enum
{
    COMPRESSION_HUFFMAN = 1,
    COMPRESSION_RLE = 2
} CompressionMethod;

typedef struct
{
    CompressionMethod method;
    size_t inputLength;
    size_t uniqueCharacters;
    size_t maximumRunLength;
    size_t huffmanBits;
    size_t rleBits;
    const char *reason;
} AdaptiveDecision;

AdaptiveDecision chooseBest(const TextAnalysis *analysis, size_t huffmanBits, size_t rleBits);

#endif
#include <stddef.h>
#include "analysis.h"
