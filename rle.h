#ifndef RLE_H
#define RLE_H

void rleEncode(char text[], char output[]);
void rleDecode(char encoded[], char output[]);
int rleBits(char encoded[]);

#endif