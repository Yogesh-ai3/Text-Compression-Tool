#include <assert.h>
#include <string.h>

#include "adaptive.h"
#include "analysis.h"
#include "huffman.h"
#include "rle.h"
#include "utils.h"

static void testRoundTrip(const char input[])
{
    TextAnalysis analysis;
    HuffmanCodeTable codes;
    EngineStatus status;
    Node *root;
    char encoded[4096];
    char decoded[1024];
    char rleEncoded[1024];
    char rleDecoded[1024];
    size_t bits;

    assert(analyseText(input, &analysis) == ENGINE_OK);
    root = buildHuffmanTree(analysis.frequency, &status);
    assert(status == ENGINE_OK);
    assert(root != NULL);
    assert(generateCodes(root, codes) == ENGINE_OK);
    assert(encodeText(input, codes, encoded, sizeof(encoded)) == ENGINE_OK);
    assert(decodeHuffman(root, encoded, decoded, sizeof(decoded)) == ENGINE_OK);
    assert(strcmp(input, decoded) == 0);
    assert(getCompressedBits(input, codes, &bits) == ENGINE_OK);
    assert(bits == strlen(encoded));
    assert(rleEncode(input, rleEncoded, sizeof(rleEncoded)) == ENGINE_OK);
    assert(rleDecode(rleEncoded, rleDecoded, sizeof(rleDecoded)) == ENGINE_OK);
    assert(strcmp(input, rleDecoded) == 0);
    freeTree(root);
}

int main(void)
{
    TextAnalysis analysis;
    char output[2];

    testRoundTrip("aaabccccdd");
    testRoundTrip("zzzz");
    testRoundTrip("abc");

    assert(analyseText("", &analysis) == ENGINE_OK);
    assert(analysis.length == 0);
    assert(analysis.uniqueCharacters == 0);
    assert(rleEncode("aaaa", output, sizeof(output)) == ENGINE_OUTPUT_TOO_SMALL);
    assert(rleDecode("a4", output, sizeof(output)) == ENGINE_OUTPUT_TOO_SMALL);
    assert(rleDecode("a0", output, sizeof(output)) == ENGINE_MALFORMED_INPUT);
    assert(chooseBest(&analysis, 0, 0).method == COMPRESSION_HUFFMAN);
    return 0;
}
