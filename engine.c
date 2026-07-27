#include "engine.h"

const char *engineStatusMessage(EngineStatus status)
{
    switch (status)
    {
    case ENGINE_OK:
        return "success";
    case ENGINE_INVALID_ARGUMENT:
        return "invalid argument";
    case ENGINE_OUTPUT_TOO_SMALL:
        return "output buffer is too small";
    case ENGINE_ALLOCATION_FAILED:
        return "memory allocation failed";
    case ENGINE_MALFORMED_INPUT:
        return "malformed input";
    }

    return "unknown error";
}
