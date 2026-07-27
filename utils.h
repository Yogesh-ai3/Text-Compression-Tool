#ifndef UTILS_H
#define UTILS_H

#include <stddef.h>
#include "engine.h"
#include "huffman.h"

EngineStatus encodeText(const char text[], const HuffmanCodeTable codes,
                        char encoded[], size_t encodedCapacity);

#endif
