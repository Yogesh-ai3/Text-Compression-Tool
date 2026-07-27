#ifndef ENGINE_H
#define ENGINE_H

#include <stddef.h>

#define ASE_SYMBOL_COUNT 256U
#define ASE_HUFFMAN_CODE_CAPACITY (ASE_SYMBOL_COUNT + 1U)

typedef enum
{
    ENGINE_OK = 0,
    ENGINE_INVALID_ARGUMENT,
    ENGINE_OUTPUT_TOO_SMALL,
    ENGINE_ALLOCATION_FAILED,
    ENGINE_MALFORMED_INPUT
} EngineStatus;

const char *engineStatusMessage(EngineStatus status);

#endif
