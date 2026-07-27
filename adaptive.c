#include "adaptive.h"

AdaptiveDecision chooseBest(const TextAnalysis *analysis, size_t huffmanBits, size_t rleBits)
{
    CompressionMethod choice = COMPRESSION_HUFFMAN;
    const char *reason = "Smaller compressed size favored Huffman.";

    if (analysis == NULL || analysis->length == 0)
    {
        return (AdaptiveDecision){
            COMPRESSION_HUFFMAN, 0, 0, 0, huffmanBits, rleBits,
            "Default fallback for empty input."
        };
    }

    if (analysis->uniqueCharacters == 1)
    {
        choice = COMPRESSION_RLE;
        reason = "Only one unique character detected.";
    }
    else if (analysis->maximumRunLength >= 4)
    {
        choice = COMPRESSION_RLE;
        reason = "Long repetition streak detected.";
    }
    else if (analysis->uniqueCharacters <= analysis->length / 4 && analysis->length > 8)
    {
        choice = COMPRESSION_RLE;
        reason = "Low symbol diversity suggests repetition-heavy data.";
    }
    else if (rleBits < huffmanBits)
    {
        choice = COMPRESSION_RLE;
        reason = "Measured compressed size favored RLE.";
    }
    else if (huffmanBits < rleBits)
    {
        choice = COMPRESSION_HUFFMAN;
        reason = "Measured compressed size favored Huffman.";
    }
    else
    {
        choice = COMPRESSION_HUFFMAN;
        reason = "Tie detected; Huffman selected as default.";
    }

    return (AdaptiveDecision){
        choice, analysis->length, analysis->uniqueCharacters,
        analysis->maximumRunLength, huffmanBits, rleBits, reason
    };
}
