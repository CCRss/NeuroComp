#!/bin/bash
set -e

if [ "$#" -ne 2 ]; then
    echo "Usage: $0 <run_number> <method>"
    exit 1
fi

RUN_NUMBER=$1
METHOD=$2
RUN_DIR="runs/run_${RUN_NUMBER}"

# Создание папки для текущего запуска
mkdir -p "${RUN_DIR}"

# Выполнение сборки и запуска eval.sh
echo "Running cmake..."
cmake .

echo "Running make..."
make

echo "Running eval.sh..."
OUTPUT=$(./eval.sh)

# Извлечение результата из вывода eval.sh
ORIGINAL_SIZE=$(echo "$OUTPUT" | grep "Original size" | awk '{print $4}')
COMPRESSED_SIZE=$(echo "$OUTPUT" | grep "Compressed size" | awk '{print $4}')
COMPRESSION_RATIO=$(echo "$OUTPUT" | grep "Compression ratio" | awk '{print $3}')
MAX_COMPRESSION_TIME=$(echo "$OUTPUT" | grep "Max compression time" | awk '{print $4}')
MAX_DECOMPRESSION_TIME=$(echo "$OUTPUT" | grep "Max decompression time" | awk '{print $4}')

# Запись результата в файл Tested.md
echo -e "\n## Run ${RUN_NUMBER}\n- **Method**: ${METHOD}\n- **Result**: ${COMPRESSION_RATIO}\n- **Max Compression Time**: ${MAX_COMPRESSION_TIME} ms\n- **Max Decompression Time**: ${MAX_DECOMPRESSION_TIME} ms\n" >> Tested.md

# Копирование исходных файлов в папку запуска
cp encode.cpp decode.cpp CMakeLists.txt "${RUN_DIR}"
cp encode decode "${RUN_DIR}"

# Вывод результатов для пользователя
echo "Original size (bytes): ${ORIGINAL_SIZE}"
echo "Compressed size (bytes): ${COMPRESSED_SIZE}"
echo "Compression ratio: ${COMPRESSION_RATIO}"
echo "Max compression time: ${MAX_COMPRESSION_TIME} ms"
echo "Max decompression time: ${MAX_DECOMPRESSION_TIME} ms"
echo "Result recorded in Tested.md and files saved in ${RUN_DIR}"
