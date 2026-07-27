#ifndef RLE_H
#define RLE_H

#include <stddef.h>
#include "engine.h"

EngineStatus rleEncode(const char text[], char output[], size_t outputCapacity);
EngineStatus rleDecode(const char encoded[], char output[], size_t outputCapacity);
size_t rleBits(const char encoded[]);

#endif
