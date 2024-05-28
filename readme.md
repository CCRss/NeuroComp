# Neuralink Compression Challenge

## Description

This competition requires the development of a compression and decompression algorithm for data obtained from Neuralink electrodes implanted in the motor cortex of a non-human primate. The recordings were made while the primate was playing a video game.

The N1 implant generates about 200 Mbps of electrode data (1024 electrodes at 20 kHz, 10-bit resolution) and can transmit ~1 Mbps wirelessly. Therefore, compression of more than 200 times is required.

Compression must be performed in real-time (< 1 ms) and consume low power (< 10 mW, including radio).

Neuralink is looking for new approaches to solve this problem and exceptional engineers to work on it. If you have a solution, submit it to compression@neuralink.com.

## Requirements

1. Build the executables `./encode` and `./decode` that pass the `eval.sh` test.
2. Compression should be lossless and achieve a high compression ratio.
3. The solution should work in real-time and consume minimal power.

## Input Data

- `data.zip` contains one hour of raw electrode recordings from the Neuralink implant.
- Required compression ratio: more than 200 times.
- Data format: mono-channel WAV files, 5 seconds per file.

## Task

1. Create the `./encode` and `./decode` executables that pass the `eval.sh` test. The script verifies lossless compression and measures the compression ratio.
2. Your work will be evaluated based on the compression ratio achieved on another set of electrode recordings. Bonus points for optimizing latency and power efficiency.
3. Submit the solution with source code and a build script, which should at least build on Linux.

## Example Output

```sh
All recordings successfully compressed.
Original size (bytes): 146800526
Compressed size (bytes): 55218715
Compression ratio: 2.65
