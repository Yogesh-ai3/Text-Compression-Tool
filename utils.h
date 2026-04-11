#ifndef UTILS_H
#define UTILS_H

#define MAX 256

void calculateFrequency(char text[], int freq[]);
void printFrequency(int freq[]);
void encodeText(char text[], char codes[256][100]);
void printStats(char text[], char codes[256][100]);

#endif