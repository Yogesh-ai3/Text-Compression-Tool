# Text-Compression-Tool
This project implements a lossless text compression system using Huffman Coding, a greedy algorithm for optimal prefix encoding.
It was developed as part of the Design & Analysis of Algorithms (DAA) coursework in B.Tech (4th Semester).

The system:
Reads input text
Builds a frequency table
Constructs a Huffman Tree
Generates optimal binary codes
Compresses the text
Decompresses it to verify correctness
Analyzes compression efficiency

Algorithm Used
Huffman Coding (Greedy Algorithm)
Huffman coding assigns variable-length binary codes to characters based on their frequency:
More frequent characters → shorter codes
Less frequent characters → longer codes
This ensures minimum weighted path length, leading to optimal compression.

Data Structures Used
Arrays (Frequency table)
Binary Trees (Huffman Tree)
Priority Queue (Min-frequency selection)
Linked structures (Tree nodes)

Time Complexity:
| Phase              | Complexity |
| ------------------ | ---------- |
| Frequency Analysis | O(n)       |
| Tree Construction  | O(k log k) |
| Encoding           | O(n)       |
| Decoding           | O(n)       |

Where:
n = length of input text,
k = number of unique characters

Compression Analysis
The program calculates:
Original size (in bits)
Compressed size (in bits)
Compression ratio

