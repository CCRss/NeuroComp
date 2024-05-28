# Tested Implementations

## Run: 1
- **Method**: Simply copies input data to output without compression.
- **Result**: Compression ratio: 0.99 (data size increased).

## Run 2
- **Method**: zlib
- **Result**: 2.24
- **Max Compression Time**: 43.241 ms
- **Max Decompression Time**: 1.1526 ms

## Run 3
- **Method**: LibLZMA
- **Result**: Fail # TODO fix error. 
    - Compression failed with error code: 10 <br>
    - Decompression failed with error code: 9

## Run 4
- **Method**: brotli
- **Result**: 2.65
- **Max Compression Time**: 593.55 ms
- **Max Decompression Time**: 2.1194 ms

## Run 5
- **Method**: flac
- **Result**: 1.48
- **Max Compression Time**: 55.8842 ms
- **Max Decompression Time**: 47.5901 ms

## Run 6
- **Method**: ppm
- **Result**: .99
- **Max Compression Time**: 322.386 ms
- **Max Decompression Time**: 271.08 ms

# TODO Something wrong with
7 run 
Running eval.sh...
Compression time: 1142.68 ms
Decompression time: 3.2483 ms
