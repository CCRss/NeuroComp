#!/bin/bash
set -e

rm -rf data
unzip data.zip

get_file_size() {
  find "$1" -printf "%s\n"
}

total_size_raw=0
encoder_size=$(get_file_size encode)
decoder_size=$(get_file_size decode)
total_size_compressed=$((encoder_size + decoder_size))

max_compression_time=0
max_decompression_time=0

for file in data/*
do
  echo "Processing $file"
  compressed_file_path="${file}.brainwire"
  decompressed_file_path="${file}.copy"

  compression_output=$(./encode "$file" "$compressed_file_path" | tee /dev/stderr)
  decompression_output=$(./decode "$compressed_file_path" "$decompressed_file_path" | tee /dev/stderr)

  file_size=$(get_file_size "$file")
  compressed_size=$(get_file_size "$compressed_file_path")

  if diff -q "$file" "$decompressed_file_path" > /dev/null; then
      echo "${file} losslessly compressed from ${file_size} bytes to ${compressed_size} bytes"
  else
      echo "ERROR: ${file} and ${decompressed_file_path} are different."
      exit 1
  fi

  total_size_raw=$((total_size_raw + file_size))
  total_size_compressed=$((total_size_compressed + compressed_size))

  compression_time=$(echo "$compression_output" | grep "Compression time" | awk '{print $3}')
  decompression_time=$(echo "$decompression_output" | grep "Decompression time" | awk '{print $3}')

  if (( $(echo "$compression_time > $max_compression_time" | bc -l) )); then
    max_compression_time=$compression_time
  fi

  if (( $(echo "$decompression_time > $max_decompression_time" | bc -l) )); then
    max_decompression_time=$decompression_time
  fi
done

compression_ratio=$(echo "scale=2; ${total_size_raw} / ${total_size_compressed}" | bc)

echo "All recordings successfully compressed."
echo "Original size (bytes): ${total_size_raw}"
echo "Compressed size (bytes): ${total_size_compressed}"
echo "Compression ratio: ${compression_ratio}"
echo "Max compression time: ${max_compression_time} ms"
echo "Max decompression time: ${max_decompression_time} ms"
