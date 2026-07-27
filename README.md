# Adaptive Storage Engine

A modular storage and text compression engine written in C.

Adaptive Storage Engine began as a Huffman Coding implementation for a Design and Analysis of Algorithms course and is being evolved into a portfolio-quality systems programming project. The long-term objective is to build a reusable storage engine capable of analysing textual data, selecting an appropriate compression strategy, storing compressed archives with metadata, and reconstructing the original data accurately.

The project focuses on software engineering principles as much as compression algorithms. Every major component is designed to be modular, extensible, and independently testable.

---

## Current Status

**Stage:** Foundation

Current implementation includes:

* Huffman Coding
* Run-Length Encoding (RLE)
* Character Frequency Analysis
* Huffman Tree Construction
* Huffman Tree Visualisation
* Text Encoding
* Text Decoding
* Compression Statistics
* Modular C Codebase

The project is currently being refactored into a reusable architecture before new functionality is added.

---

## Project Goals

The long-term vision is to build an adaptive storage engine capable of:

* Analysing input characteristics
* Selecting an appropriate compression algorithm
* Supporting multiple compression strategies
* Storing compressed archives together with metadata
* Restoring the original data without loss
* Explaining why a compression strategy was selected
* Benchmarking compression performance
* Supporting future AI-assisted strategy selection

---

## Current Project Structure

```text
.
├── adaptive.c
├── adaptive.h
├── huffman.c
├── huffman.h
├── rle.c
├── rle.h
├── utils.c
├── utils.h
├── main.c
├── README.md
└── ROADMAP.md
```

As the project grows, additional modules such as file analysis, archive management, benchmarking, testing, and strategy selection will be introduced while keeping responsibilities separated.

---

## Algorithms

### Huffman Coding

A lossless compression algorithm that assigns shorter binary codes to more frequent characters and longer codes to less frequent characters.

### Run-Length Encoding

A simple lossless compression algorithm that replaces repeated sequences with a value and its repetition count.

Future releases will support combining algorithms into compression pipelines.

---

## Software Engineering Objectives

This project is intended to demonstrate:

* Modular software architecture
* Separation of concerns
* Maintainable C code
* Binary file handling
* Performance analysis
* Memory management
* Extensible system design
* Professional project documentation

---

## Future Architecture


                Input File
                     │
                     ▼
              File Analysis
                     │
                     ▼
         Compression Strategy Engine
           │        │          │
           ▼        ▼          ▼
        Huffman    RLE     Future Algorithms
                     │
                     ▼
              Archive Builder
                     │
                     ▼
          Compressed Storage Format
                     │
                     ▼
              Decompression Engine
```

---

## Build

Use the included build targets:

```bash
make          # build/adaptive_storage_engine
make run      # run the interactive demonstration
make test     # execute regression and API-contract tests
```

## Current Architecture

The executable is deliberately an interactive demonstration layer; it is not the
compression library itself. `main.c` owns terminal input, formatting, and error
messages. Core modules return data through explicit outputs and status codes, so
they can be reused later by a file CLI, archive writer, or benchmark runner.

| Module | Responsibility | Does not own |
| --- | --- | --- |
| `engine` | Shared status vocabulary and common limits | Compression policy or I/O |
| `analysis` | One-pass input characteristics and frequency table | Algorithm selection or rendering |
| `huffman` | Tree lifecycle, code generation, Huffman encode/decode support | Terminal output |
| `rle` | RLE encode/decode and size calculation | Strategy selection or terminal output |
| `adaptive` | Rule-based selection from `TextAnalysis` and measured sizes | Re-scanning input or printing |
| `utils` | Huffman bitstream assembly | Presentation or analysis |
| `main` | Demonstration workflow and presentation | Compression implementation |

### API Contract

Public transformation APIs accept an output buffer and its capacity, then return
an `EngineStatus`. This makes ownership and failure behaviour explicit:

* `ENGINE_OK` means a NUL-terminated result was written.
* `ENGINE_OUTPUT_TOO_SMALL` means no caller should rely on a complete result.
* `ENGINE_MALFORMED_INPUT` identifies invalid encoded data or missing codes.
* `ENGINE_ALLOCATION_FAILED` reports Huffman tree allocation failure.

`TextAnalysis` centralizes the frequency table, length, distinct-symbol count,
and maximum run length. Huffman construction and adaptive selection now consume
the same analysis rather than each scanning the source independently.

The Huffman `Node` remains visible only because the existing tree visualization
needs read-only access in the presentation layer. Hiding it behind an opaque
tree plus a visitor/renderer interface is intentionally deferred: it would add
an abstraction before the project has another consumer that needs it.

The current RLE text representation is retained for compatibility with the
original demonstration. It is not yet an archive-safe representation for every
possible byte value; archive serialization belongs to the planned archive-format
milestone rather than this architecture-only refactor.

---

## Future Work

The development plan is maintained in `ROADMAP.md`.

Each milestone will preserve backward compatibility while incrementally improving the architecture and feature set.

---

## Educational Purpose

The primary objective of this project is learning systems programming and software engineering through incremental development. Every architectural decision is expected to be documented and justified rather than implemented solely to add features.
